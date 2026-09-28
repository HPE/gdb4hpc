/******************************************************************************\
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "dbgclnt_proto_ctiship.h"
#include "dbg_client.h"
#include "useful.h"

#include <filesystem>

std::optional<std::filesystem::path> getShareDir() {
    auto exe = std::filesystem::path{};
    try {
        exe = std::filesystem::canonical("/proc/self/exe");
    } catch (std::exception& err) {
        return std::nullopt;
    }

    // expecting to get <install dir>/bin/gdb4hpc and expecting <install dir>/share
    auto base = exe.parent_path().parent_path();
    return base / "share";
}

std::optional<std::string> shipGdb(cti_manifest_id_t manifest,
                                   const std::string& gdb,
                                   const std::string& gdbDataDir) {
    auto serverGdb = std::string{"gdb-guard"};

    // user can specify a custom gdb binary and a datadir
    if (!gdb.empty()) {
        auto dataDir = gdbDataDir;
        
        if (!dataDir.empty() && dataDir[0] != '/') {
            auto dataPath = std::filesystem::current_path() / dataDir;
            dataDir = dataPath;
        }

        if (gdb[0] == '/') {
            serverGdb = gdb;
        } else {
            serverGdb = std::filesystem::path(gdb).filename();
        }

        if (cti_addManifestBinary(manifest, gdb.c_str())) {
            PrintOutError("Failed to add %s to manifest.\n", gdb.c_str());
            PrintOutError("CTI error: %s\n", cti_error_str());
            return {};
        }

        if (!dataDir.empty()) {
            serverGdb += ":";
            serverGdb += dataDir;
        }
    } else if (pathHas(serverGdb.c_str())) {
        if (cti_addManifestBinary(manifest, serverGdb.c_str())) {
            PrintOutError("Failed to add gdb-guard to manifest.\n");
            PrintOutError("CTI error: %s\n", cti_error_str());
            return {};
        }
    } else if (pathFind("gdb")) {
        serverGdb = "gdb";
    } else {
        PrintOutError("gdb is not installed");
        return {};
    }

    return serverGdb;
}

// Add the gpu enabled version of gdb to the manifest.  The back-end will determine
// which one to use; and they might well not be the same for each rank.
bool shipGpuGdbs(cti_manifest_id_t manifest) {
    // The backend code will determine which version of cuda-gdb to use
    //if (cti_addManifestBinary(manifest, "cuda110-gdb-guard")) {
    if (char* cudaGdb = pathFind("cuda-gdb")) {
        if (cti_addManifestBinary(manifest, cudaGdb)) {
            return false;
        }
        free(cudaGdb);
    }

    if (cti_addManifestBinary(manifest, "cuobjdump")) {
        // cuda-gdb relies on cuobjdump from cudatoolkit.   If the user is working on an
        // amd system, it is not required so printint a complaint here could be confusing.
        // It will be cleaner to trigger this from the back-end after it determines we
        // are on an nvidia system.
        //PrintOutError("Failed to add cuobjdump to manifest.\n");
    }

    // For rocgdb we rely on a system install of rocgdb, currently rocgdb is updating
    // continuously and a typical machine might have 3 or 4 versions available.   So it
    // make sense to just let the user select the rocgdb version they want by
    // loading the right module or otherwise manipulationg PATH.
    if (char* rocgdb = pathFind("rocgdb")) {
        if (cti_addManifestBinary(manifest, rocgdb)) {
            free(rocgdb);
            return false;
        }
        free(rocgdb);
    }

    if (char* rocminfo = pathFind("rocminfo")) {
        if (cti_addManifestBinary(manifest, rocminfo)) {
            free(rocminfo);
            return false;
        }
        free(rocminfo);
    }


    return true;
}

// Explicitly publish the ompd library files for the back-end.
bool shipOmpd(cti_manifest_id_t mid) {
    auto maybe_share_dir = getShareDir();
    if (!maybe_share_dir) {
        PrintOutMsg("Could not get executable path, OMPD support will not be available\n");
        return false;
    }
    auto ompd_dir = *maybe_share_dir / "ompd/gdb";

    try {
        for (const auto& entry : std::filesystem::directory_iterator(ompd_dir)) {
            auto f = entry.path();
            if (f.filename() == "__pycache__") continue;
            if (cti_addManifestFile(mid, f.c_str())) {
                PrintOutMsg("Adding OMPD file %s to manifest failed\n", f.c_str());
                return false;
            }
        }
    } catch (std::exception& err) {
        PrintOutMsg("OMPD path %s not found, OMPD will not be available\n", ompd_dir.c_str());
        return false;
    }

    return true;
}

bool shipCommonFiles(cti_manifest_id_t manifest) {
    if (cti_addManifestBinary(manifest, "dbgsrv")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add dbgsrv to manifest.\n");
        return false;
    }

    // Add libthread_db otherwise gdb will be messed up with thread support
    if (cti_addManifestLibrary(manifest, "libthread_db.so.1")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add libthread_db.so.1 to manifest.\n");
        return false;
    }

    return true;
}
