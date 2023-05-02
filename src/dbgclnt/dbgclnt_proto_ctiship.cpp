/******************************************************************************\
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "dbgclnt_proto_ctiship.h"
#include "dbg_client.h"
#include "useful.h"

#include <filesystem>

std::optional<std::string> shipGdb(cti_manifest_id_t manifest,
                                   const std::string& gdb,
                                   const std::string& gdbDataDir) {
    auto serverGdb = std::string{"gdb-guard"};

    // user can specify a custom gdb binary and a (full path) datadir
    if (!gdb.empty()) {
        if (!gdbDataDir.empty() && gdbDataDir[0] != '/') {
            PrintOutError("adding a dataDir to the manifest is not supported.\n");
            return {};
        }

        if (gdb[0] == '/') {
            serverGdb = gdb;
        } else {
            if (cti_addManifestBinary(manifest, gdb.c_str())) {
                PrintOutError("Failed to add %s to manifest.\n", gdb.c_str());
                PrintOutError("CTI error: %s\n", cti_error_str());
                return {};
            }
            serverGdb = std::filesystem::path(gdb).filename();
        }

        if (!gdbDataDir.empty()) {
            serverGdb += ":";
            serverGdb += gdbDataDir;
        }
    } else if (cti_addManifestBinary(manifest, serverGdb.c_str())) {
        PrintOutError("Failed to add gdb-guard to manifest.\n");
        PrintOutError("CTI error: %s\n", cti_error_str());
        return {};
    }

    return serverGdb;
}

// Add the gpu enabled version of gdb to the manifest.  The back-end will determine
// which one to use; and they might well not be the same for each rank.
bool shipGpuGdbs(cti_manifest_id_t manifest) {
    // The backend code will determine which version of cuda-gdb to use
    //if (cti_addManifestBinary(manifest, "cuda110-gdb-guard")) {
    if (char* cudaGdb = pathFind("cuda-gdb", nullptr)) {
        if (cti_addManifestBinary(manifest, "cuda-gdb")) {
            free(cudaGdb);
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
    if (char* rocgdb = pathFind("rocgdb", nullptr)) {
        if (cti_addManifestBinary(manifest, rocgdb)) {
            free(rocgdb);
            return false;
        }
        free(rocgdb);
    }

    if (char* rocminfo = pathFind("rocminfo", nullptr)) {
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
    auto dir = std::string{};
    try {
        dir = std::filesystem::canonical("/proc/self/exe");
    } catch (std::exception& err) {
        PrintOutMsg("Could not get executable path, OMPD support will not be available\n");
        return false;
    }


    // we expect gdb4hpc to bin a bin directory and ompd to be in share
    auto bloc = dir.rfind("/bin");

    if (bloc == std::string::npos) return false;
    dir.resize(bloc+1); // keep the /
    dir += "share/ompd/gdb";

    try {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            auto f = entry.path();
            if (f.filename() == "__pycache__") continue;
            if (cti_addManifestFile(mid, f.c_str())) {
                PrintOutMsg("Adding OMPD file %s to manifest failed\n", f.c_str());
                return false;
            }
        }
    } catch (std::exception& err) {
        PrintOutMsg("OMPD path %s not found, OMPD will not be available\n", dir.c_str());
        return false;
    }

    return true;
}

bool shipCommonFiles(cti_manifest_id_t manifest, bool gpu_flag) {
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
