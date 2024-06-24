/******************************************************************************\
 * Copyright 2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dbginvoke.h"
#include "dbgclnt_proto.h"
#include "dbg_client.h"
#include "path.h"
#include "str.h"
#include "dense_bitset.h"

#include <getopt.h>
#include <algorithm>
#include <fstream>
#include <filesystem>

#include "unittest_macros.h"

TEST_CASE("DbgInvoke: operator==") {
    // operator== relies on the DbgInvoke having a handle. DbgInvokes get handles by
    // registering themselves in the global DbgInvoke list.

    auto Launched = DbgInvoke::ProcessType::Launched;
    auto di = std::make_unique<DbgInvoke>(Launched, 4, *findClientProtoByName("zmqnet"));
    auto di2 = std::make_unique<DbgInvoke>(Launched, 4, *findClientProtoByName("zmqnet"));

    auto dih = DbgInvoke::activate("test_di", std::move(di));
    auto dih2 = DbgInvoke::activate("test_di_2", std::move(di2));

    SUBCASE("operator==") {
        auto dip = dih.lock();
        auto dip2 = dih.lock(); // same di
        REQUIRE(dip != nullptr);
        REQUIRE(dip2 != nullptr);

        CHECK(*dip == *dip2);
    }

    SUBCASE("operator!=") {
        auto dip = dih.lock();
        auto dip2 = dih2.lock(); // different di
        REQUIRE(dip != nullptr);
        REQUIRE(dip2 != nullptr);

        CHECK(*dip != *dip2);
    }

    // clean up as to not affect other tests
    DbgInvoke::deactivateAll();
}

TEST_CASE("DbgInvoke: set/get/unset frontendGdb") {
    using DI = DbgInvoke;
    auto di = DbgInvoke(DI::ProcessType::Launched, 4, *findClientProtoByName("zmqnet"));

    REQUIRE(di.frontendGdb() == nullptr);

    SUBCASE("Instantiate, deinstantiate with nullptr") {
        auto r = di.instantiateFrontendGdb(nullptr);
        CHECK(r != nullptr);
        REQUIRE(di.frontendGdb() != nullptr);

        di.resetFrontendGdb();
        CHECK(di.frontendGdb() == nullptr);
    }
}

TEST_CASE("DbgInvoke: set/call/unset event callback function") {
    auto fn = [](DbgInvoke::EventSet&, const Bitset&) {
        return 1;
    };
    auto Launched = DbgInvoke::ProcessType::Launched;
    auto di = DbgInvoke(Launched, 4, *findClientProtoByName("zmqnet"));
    auto bs = DenseBitset(8);
    auto es = DbgInvoke::EventSet(di, bs, nullptr);

    REQUIRE(di.getAsyncEventCallback() == nullptr);

    // set
    di.setAsyncEventCallback(fn);
    REQUIRE(di.getAsyncEventCallback() != nullptr);
    CHECK(di.callAsyncEventCallback(es, bs) == 1);

    // unset
    di.setAsyncEventCallback(nullptr);
    REQUIRE(di.getAsyncEventCallback() == nullptr);
    CHECK_THROWS_AS(di.callAsyncEventCallback(es, bs), std::bad_function_call);
}

TEST_CASE("DbgInvoke: check that no other tests polluted global registry") {
    CHECK(DbgInvoke::getAll().size() == 0);
}

TEST_CASE("DbgInvoke: Parse LaunchOpts") {
    SUBCASE("Throws exception when missing required args") {
        const char* const argv[] = {};
        auto csi = MockClientSettings();

        CHECK_THROWS(LaunchOpts(0, argv, csi));
    }

    SUBCASE("Settings from global client settings") {
        const char* const argv[] = {
            "launch", "$a{1}", "/tmp/c_inc",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        CHECK(opts.unsafe_flag == false);
        CHECK(opts.non_stop_flag == true);
        CHECK(opts.early_entry_flag == true);
        CHECK(opts.proto_name == "mock_zmqnet");
        CHECK(opts.backend_name == "mock_gdbmi");
    }

    SUBCASE("Path settings: unquoted") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",

            "--launcher=/tmp/fake_mpiexec",
            "--launcher-input=/tmp/fake_input",
            "--gdb=/tmp/fake_gdb",
            "--workdir=/tmp/fake_workdir",
            "--qsub=/tmp/fake_qsub",
            "--sbatch=/tmp/fake_sbatch",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.job_opts.size() == 1);
        CHECK(opts.launcher_name == "/tmp/fake_mpiexec");
        CHECK(opts.input_file_path == "/tmp/fake_input");
        CHECK(opts.gdb_path == "/tmp/fake_gdb");
        CHECK(opts.cwd == "/tmp/fake_workdir");
        CHECK(opts.qsub_file_path == "/tmp/fake_qsub");
        CHECK(opts.sbatch_file_path == "/tmp/fake_sbatch");
    }

    SUBCASE("Path settings: quoted") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",

            "-l", "\"/tmp/fake mpiexec\"",
            "-i", "\"/tmp/fake input\"",
            "-b", "\"/tmp/fake gdb\"",
            // since --workdir is valid even without a passed path,
            // the argument parser treats any "-d" argument as having
            // no argument, even when followed by a string
            "--workdir=\"/tmp/fake workdir\"",
            "-q", "\"/tmp/fake qsub\"",
            "-s", "\"/tmp/fake sbatch\"",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        // quotes should be removed
        REQUIRE(opts.job_opts.size() == 1);
        CHECK(opts.launcher_name == "/tmp/fake mpiexec");
        CHECK(opts.input_file_path == "/tmp/fake input");
        CHECK(opts.gdb_path == "/tmp/fake gdb");
        CHECK(opts.cwd == "/tmp/fake workdir");
        CHECK(opts.qsub_file_path == "/tmp/fake qsub");
        CHECK(opts.sbatch_file_path == "/tmp/fake sbatch");
    }

    SUBCASE("Binary flags") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",
            "--gpu", "--non-mpi", "--debug", "--workdir",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        CHECK(opts.gpu_flag == true);
        CHECK(opts.debug_flag == true);
        CHECK(opts.set_default_cwd == true);
    }

    SUBCASE("Launcher args") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",
            "--launcher=/tmp/fake_mpiexec",
            "--launcher-args=\"-n3 -N1 --exclusive \"multi word arg\"\"",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.job_opts.size() == 1);
        REQUIRE(opts.job_opts[0].launcher_args.argc() == 4);
        CHECK(opts.job_opts[0].launcher_args[0] == "-n3");
        CHECK(opts.job_opts[0].launcher_args[1] == "-N1");
        CHECK(opts.job_opts[0].launcher_args[2] == "--exclusive");
        CHECK(opts.job_opts[0].launcher_args[3] == "multi word arg"); // should have no quotes
    }

    SUBCASE("App args") {
        SUBCASE("via --args") {
            const char* const argv[] = {
                "launch", "$a{1}", "./bin/c_inc",
                "--args=\"input_file output_file 44 \"multi word arg\"\"",
            };
            auto csi = MockClientSettings();
            auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

            REQUIRE(opts.job_opts.size() == 1);
            REQUIRE(opts.job_opts[0].app_argv.argc() == 5);
            REQUIRE(opts.job_opts[0].run_argv.argc() == 5);

            CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].app_argv[1] == "input_file");
            CHECK(opts.job_opts[0].app_argv[2] == "output_file");
            CHECK(opts.job_opts[0].app_argv[3] == "44");
            CHECK(opts.job_opts[0].app_argv[4] == "multi word arg"); // should have no quotes

            CHECK(opts.job_opts[0].run_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].run_argv[1] == "input_file");
            CHECK(opts.job_opts[0].run_argv[2] == "output_file");
            CHECK(opts.job_opts[0].run_argv[3] == "44");
            CHECK(opts.job_opts[0].run_argv[4] == "multi word arg");
        }

        SUBCASE("via positional args") {
            const char* const argv[] = {
                "launch", "$a{1}", "./bin/c_inc",
                "input_file", "output_file", "44", "\"multi word arg\"",
            };
            auto csi = MockClientSettings();
            auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

            REQUIRE(opts.job_opts.size() == 1);
            REQUIRE(opts.job_opts[0].app_argv.argc() == 5);
            REQUIRE(opts.job_opts[0].run_argv.argc() == 5);

            CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].app_argv[1] == "input_file");
            CHECK(opts.job_opts[0].app_argv[2] == "output_file");
            CHECK(opts.job_opts[0].app_argv[3] == "44");
            CHECK(opts.job_opts[0].app_argv[4] == "multi word arg"); // should have no quotes

            CHECK(opts.job_opts[0].run_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].run_argv[1] == "input_file");
            CHECK(opts.job_opts[0].run_argv[2] == "output_file");
            CHECK(opts.job_opts[0].run_argv[3] == "44");
            CHECK(opts.job_opts[0].run_argv[4] == "multi word arg");
        }

        SUBCASE("via positional args with --") {
            const char* const argv[] = {
                "launch", "$a{1}", "./bin/c_inc",
                "--", "input_file", "output_file", "44", "\"multi word arg\"",
            };
            auto csi = MockClientSettings();
            auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

            REQUIRE(opts.job_opts.size() == 1);
            REQUIRE(opts.job_opts[0].app_argv.argc() == 5);
            REQUIRE(opts.job_opts[0].run_argv.argc() == 5);

            CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].app_argv[1] == "input_file");
            CHECK(opts.job_opts[0].app_argv[2] == "output_file");
            CHECK(opts.job_opts[0].app_argv[3] == "44");
            CHECK(opts.job_opts[0].app_argv[4] == "multi word arg"); // should have no quotes

            CHECK(opts.job_opts[0].run_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].run_argv[1] == "input_file");
            CHECK(opts.job_opts[0].run_argv[2] == "output_file");
            CHECK(opts.job_opts[0].run_argv[3] == "44");
            CHECK(opts.job_opts[0].run_argv[4] == "multi word arg");
        }

        SUBCASE("Catch when both -a and positional app args are used") {
            const char* const argv[] = {
                "launch", "$a{1}", "./bin/c_inc",
                "--args=\"too many args!\"",
                "input_file", "output_file", "44", "\"multi word arg\"",
            };
            auto csi = MockClientSettings();
            CHECK_THROWS(LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi));
        }
    }

    SUBCASE("MPMD args") {
        SUBCASE("via --args") {
            const char* const argv[] = {
                "launch", "$a{1}",
                "--launcher=/tmp/fake_mpiexec",
                "--launcher-args=\"-n1 -N1\"",  "./bin/c_inc", "--non-mpi", "--args=\"inc_arg1 inc_arg2\"", ":",
                "--launcher-args=\"-n2 -N2\"",  "./bin/c_dec", "--args=\"dec_arg1 dec_arg2\"",
            };
            auto csi = MockClientSettings();
            auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

            CHECK(opts.launcher_name == "/tmp/fake_mpiexec");
            REQUIRE(opts.job_opts.size() == 2);
            CHECK(opts.job_opts[0].non_mpi_flag == true);
            REQUIRE(opts.job_opts[0].launcher_args.argc() == 2);
            CHECK(opts.job_opts[0].launcher_args[0] == "-n1");
            CHECK(opts.job_opts[0].launcher_args[1] == "-N1");
            REQUIRE(opts.job_opts[0].app_argv.argc() == 3);
            CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].app_argv[1] == "inc_arg1");
            CHECK(opts.job_opts[0].app_argv[2] == "inc_arg2");
            CHECK(opts.job_opts[1].non_mpi_flag == false);
            REQUIRE(opts.job_opts[1].launcher_args.argc() == 2);
            CHECK(opts.job_opts[1].launcher_args[0] == "-n2");
            CHECK(opts.job_opts[1].launcher_args[1] == "-N2");
            REQUIRE(opts.job_opts[1].app_argv.argc() == 3);
            CHECK(opts.job_opts[1].app_argv[0] == "./bin/c_dec");
            CHECK(opts.job_opts[1].app_argv[1] == "dec_arg1");
            CHECK(opts.job_opts[1].app_argv[2] == "dec_arg2");
        }

        SUBCASE("via positional args") {
            const char* const argv[] = {
                "launch", "$a{1}",
                "--launcher=/tmp/fake_mpiexec",
                "--launcher-args=\"-n1 -N1\"",  "./bin/c_inc", "--non-mpi", "inc_arg1", "inc_arg2", ":",
                "--launcher-args=\"-n2 -N2\"",  "./bin/c_dec", "dec_arg1", "dec_arg2",
            };
            auto csi = MockClientSettings();
            auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

            CHECK(opts.launcher_name == "/tmp/fake_mpiexec");
            REQUIRE(opts.job_opts.size() == 2);
            CHECK(opts.job_opts[0].non_mpi_flag == true);
            REQUIRE(opts.job_opts[0].launcher_args.argc() == 2);
            CHECK(opts.job_opts[0].launcher_args[0] == "-n1");
            CHECK(opts.job_opts[0].launcher_args[1] == "-N1");
            REQUIRE(opts.job_opts[0].app_argv.argc() == 3);
            CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].app_argv[1] == "inc_arg1");
            CHECK(opts.job_opts[0].app_argv[2] == "inc_arg2");
            CHECK(opts.job_opts[1].non_mpi_flag == false);
            REQUIRE(opts.job_opts[1].launcher_args.argc() == 2);
            CHECK(opts.job_opts[1].launcher_args[0] == "-n2");
            CHECK(opts.job_opts[1].launcher_args[1] == "-N2");
            REQUIRE(opts.job_opts[1].app_argv.argc() == 3);
            CHECK(opts.job_opts[1].app_argv[0] == "./bin/c_dec");
            CHECK(opts.job_opts[1].app_argv[1] == "dec_arg1");
            CHECK(opts.job_opts[1].app_argv[2] == "dec_arg2");
        }

        SUBCASE("one -a and one positional") {
            const char* const argv[] = {
                "launch", "$a{1}",
                "--launcher=/tmp/fake_mpiexec",
                "--launcher-args=\"-n1 -N1\"",  "./bin/c_inc", "--non-mpi", "inc_arg1", "inc_arg2", ":",
                "--launcher-args=\"-n2 -N2\"",  "./bin/c_dec", "--args=\"dec_arg1 dec_arg2\"",
            };
            auto csi = MockClientSettings();
            auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

            CHECK(opts.launcher_name == "/tmp/fake_mpiexec");
            REQUIRE(opts.job_opts.size() == 2);
            CHECK(opts.job_opts[0].non_mpi_flag == true);
            REQUIRE(opts.job_opts[0].launcher_args.argc() == 2);
            CHECK(opts.job_opts[0].launcher_args[0] == "-n1");
            CHECK(opts.job_opts[0].launcher_args[1] == "-N1");
            REQUIRE(opts.job_opts[0].app_argv.argc() == 3);
            CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].app_argv[1] == "inc_arg1");
            CHECK(opts.job_opts[0].app_argv[2] == "inc_arg2");
            CHECK(opts.job_opts[1].non_mpi_flag == false);
            REQUIRE(opts.job_opts[1].launcher_args.argc() == 2);
            CHECK(opts.job_opts[1].launcher_args[0] == "-n2");
            CHECK(opts.job_opts[1].launcher_args[1] == "-N2");
            REQUIRE(opts.job_opts[1].app_argv.argc() == 3);
            CHECK(opts.job_opts[1].app_argv[0] == "./bin/c_dec");
            CHECK(opts.job_opts[1].app_argv[1] == "dec_arg1");
            CHECK(opts.job_opts[1].app_argv[2] == "dec_arg2");
        }

        SUBCASE("one -a and one positional (other permutation)") {
            const char* const argv[] = {
                "launch", "$a{1}",
                "--launcher=/tmp/fake_mpiexec",
                "--launcher-args=\"-n1 -N1\"",  "./bin/c_inc", "--non-mpi", "--args=\"inc_arg1 inc_arg2\"", ":",
                "--launcher-args=\"-n2 -N2\"",  "./bin/c_dec", "dec_arg1", "dec_arg2",
            };
            auto csi = MockClientSettings();
            auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

            CHECK(opts.launcher_name == "/tmp/fake_mpiexec");
            REQUIRE(opts.job_opts.size() == 2);
            CHECK(opts.job_opts[0].non_mpi_flag == true);
            REQUIRE(opts.job_opts[0].launcher_args.argc() == 2);
            CHECK(opts.job_opts[0].launcher_args[0] == "-n1");
            CHECK(opts.job_opts[0].launcher_args[1] == "-N1");
            REQUIRE(opts.job_opts[0].app_argv.argc() == 3);
            CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc");
            CHECK(opts.job_opts[0].app_argv[1] == "inc_arg1");
            CHECK(opts.job_opts[0].app_argv[2] == "inc_arg2");
            CHECK(opts.job_opts[1].non_mpi_flag == false);
            REQUIRE(opts.job_opts[1].launcher_args.argc() == 2);
            CHECK(opts.job_opts[1].launcher_args[0] == "-n2");
            CHECK(opts.job_opts[1].launcher_args[1] == "-N2");
            REQUIRE(opts.job_opts[1].app_argv.argc() == 3);
            CHECK(opts.job_opts[1].app_argv[0] == "./bin/c_dec");
            CHECK(opts.job_opts[1].app_argv[1] == "dec_arg1");
            CHECK(opts.job_opts[1].app_argv[2] == "dec_arg2");
        }

        SUBCASE("Catch when both -a and positional app args are used in one argument set") {
            const char* const argv[] = {
                "launch", "$a{1}",
                "--launcher=/tmp/fake_mpiexec",
                "--launcher-args=\"-n1 -N1\"",  "./bin/c_inc", "--non-mpi", "inc_arg1", "inc_arg2", ":",
                "--launcher-args=\"-n2 -N2\"",  "./bin/c_dec", "--args=\"dec_arg1 dec_arg2\"", "this-is-a-bad-argument",
            };
            auto csi = MockClientSettings();
            CHECK_THROWS(LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi));
        }
    }

    SUBCASE("python") {
        SUBCASE("automatically detect python3") {
            SUBCASE ("python3") {
                const char* const argv[] = {
                    "launch", "$a{1}", "python3",
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            SUBCASE ("python3.11") {
                const char* const argv[] = {
                    "launch", "$a{1}", "python3.11",
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            SUBCASE ("/usr/bin/python3") {
                const char* const argv[] = {
                    "launch", "$a{1}", "/usr/bin/python3",
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            SUBCASE ("cray python") {
                const char* const argv[] = {
                    "launch", "$a{1}", "/global/opt/cray/pe/python/3.11.7/bin/python3.11",
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
        }

        SUBCASE("manual --python flag") {
            {   // ensure we aren't detecting python3 when it isn't there
                const char* const argv[] = {
                    "launch", "$a{1}", "./i-promise-this-is-pyth0n.x"
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == false);
            }
            {   // i promise it's there!
                const char* const argv[] = {
                    "launch", "$a{1}", "./i-promise-this-is-pyth0n.x", "--python"
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            {   // different order
                const char* const argv[] = {
                    "launch", "$a{1}", "--python", "./i-promise-this-is-pyth0n.x",
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            {   // short form
                const char* const argv[] = {
                    "launch", "$a{1}", "./i-promise-this-is-pyth0n.x", "-p"
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
        }

        SUBCASE("manual --no-python flag") {
            {
                const char* const argv[] = {
                    "launch", "$a{1}", "python3", "--no-python",
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == false);
            }
            {   // different order
                const char* const argv[] = {
                    "launch", "$a{1}", "--no-python", "python3",
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == false);
            }
            {   // short form
                const char* const argv[] = {
                    "launch", "$a{1}", "-P", "python3",
                };
                auto csi = MockClientSettings();
                auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == false);
            }
        }
    }
}

struct MockFs {
    MockFs() {
        if (std::filesystem::exists("./mockfs")) {
            throw std::runtime_error("./mockfs exists");
        }

        std::filesystem::create_directory("./mockfs");
        std::ofstream{"./mockfs/unittest_binary"};
        std::ofstream{"./mockfs/unittest_gdb"};
        std::ofstream{"./mockfs/unittest_txt"};
    }
    ~MockFs() {
        std::filesystem::remove_all("./mockfs");
    }

    static std::unique_ptr<MockFs> make() {
        try {
            return std::make_unique<MockFs>();
        } catch (const std::exception &e) {
            return nullptr;
        }
    }
};

TEST_CASE("DbgInvoke: LaunchArgs enforceValid() validates that files are present") {
    if (auto fs = MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--workdir", "--gdb=unittest_gdb",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        using path = std::filesystem::path;

        auto cwd = std::filesystem::current_path();
        REQUIRE(std::filesystem::exists(cwd / path{"mockfs"}));

        // contains unittest_gdb
        setenv("PATH", "./mockfs", 1);

        REQUIRE_NOTHROW(opts.enforceValid());

        CHECK(opts.job_opts[0].app_argv[0] == cwd / path{"mockfs/unittest_binary"});
        CHECK(opts.cwd == cwd / path{"mockfs"});
        CHECK(opts.gdb_path == cwd / path{"mockfs/unittest_gdb"});
    } else {
        WARN_MESSAGE(false, "Failed to create mockfs");
    }
}

TEST_CASE("DbgInvoke: launch args enforceValid replaces relative paths with full paths") {
    if (auto fs = MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",

            "--launcher=./mockfs/unittest_binary",
            "--launcher-input=./mockfs/unittest_txt",
            "--gdb=./mockfs/unittest_gdb",
            "--workdir=./mockfs",
            "--qsub=./mockfs/unittest_txt",
            "--sbatch=./mockfs/unittest_txt",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        using path = std::filesystem::path;

        auto cwd = std::filesystem::current_path();
        REQUIRE(std::filesystem::exists(cwd / path{"mockfs"}));

        REQUIRE_NOTHROW(opts.enforceValid());

        CHECK(opts.launcher_name == cwd / path{"mockfs/unittest_binary"});
        CHECK(opts.job_opts[0].app_argv[0] == cwd / path{"mockfs/unittest_binary"});
        CHECK(opts.job_opts[0].run_argv[0] == cwd / path{"mockfs/unittest_binary"});
        CHECK(opts.input_file_path == cwd / path{"mockfs/unittest_txt"});
        CHECK(opts.gdb_path == cwd / path{"mockfs/unittest_gdb"});
        CHECK(opts.cwd == cwd / path{"mockfs"});
        CHECK(opts.qsub_file_path == cwd / path{"mockfs/unittest_txt"});
        CHECK(opts.sbatch_file_path == cwd / path{"mockfs/unittest_txt"});
    } else {
        WARN_MESSAGE(false, "Failed to create mockfs");
    }
}

TEST_CASE("DbgInvoke: LaunchArgs --gdb=/path/to/gdb:/path/to/datadir") {
    if (auto fs = MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--gdb=./mockfs/unittest_gdb:./mockfs",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        using path = std::filesystem::path;

        auto cwd = std::filesystem::current_path();
        REQUIRE(std::filesystem::exists(cwd / path{"mockfs"}));

        REQUIRE_NOTHROW(opts.enforceValid());

        // something like /path/to/mockfs/unittest_gdb:/path/to/mockfs
        auto full_paths = std::string{cwd} + "/mockfs/unittest_gdb:" + std::string{cwd} + "/mockfs";
        CHECK(opts.gdb_path == full_paths);
    } else {
        WARN_MESSAGE(false, "Failed to create mockfs");
    }
}

TEST_CASE("DbgInvoke: LaunchArgs --gdb=/path/to/gdb: edge case") {
    if (auto fs = MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--gdb=./mockfs/unittest_gdb:",
        };
        auto csi = MockClientSettings();
        auto opts = LaunchOpts(sizeof(argv)/sizeof(argv[0]), argv, csi);

        using path = std::filesystem::path;

        auto cwd = std::filesystem::current_path();
        REQUIRE(std::filesystem::exists(cwd / path{"mockfs"}));

        REQUIRE_NOTHROW(opts.enforceValid());

        // colon is removed
        CHECK(opts.gdb_path == std::string{cwd} + "/mockfs/unittest_gdb");
    } else {
        WARN_MESSAGE(false, "Failed to create mockfs");
    }
}

TEST_CASE("rep_set_t and group_recv operators") {
    // rep_set_t::operator== uses the operator== of the contained group_recvs.
    // That operator in turn relies on DbgInvoke::== and Bitset::==.
    //
    // DbgInvoke::== relies on the DbgInvoke having a handle. DbgInvokes get
    // handles by registering themselves in the global DbgInvoke list.

    auto Launched = DbgInvoke::ProcessType::Launched;
    auto di0 = std::make_unique<DbgInvoke>(Launched, 3, *findClientProtoByName("zmqnet"));
    auto di1 = std::make_unique<DbgInvoke>(Launched, 4, *findClientProtoByName("zmqnet"));
    auto dih0 = DbgInvoke::activate("test_di_0", std::move(di0));
    auto dih1 = DbgInvoke::activate("test_di_1", std::move(di1));

    const auto make_bitset = [](size_t size, size_t first, size_t second) {
        auto bitset = std::make_unique<DenseBitset>(size);
        bitset->set(first);
        bitset->set(second);
        return bitset;
    };

    auto bs_011 = make_bitset(3, 0, 1);
    auto bs_110 = make_bitset(3, 1, 2);
    auto bs_101 = make_bitset(3, 0, 2);
    auto bs_0110 = make_bitset(4, 1, 2);

    // pointers are used for these tests because the rec_set_t code is pointer based.

    auto di0_011 = std::make_unique<group_recv>(
        group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_011->clone() });
    auto di0_110 = std::make_unique<group_recv>(
        group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_110->clone() });
    auto di1_0110 = std::make_unique<group_recv>(
        group_recv{ .rec_di = *dih1.lock(), .rec_group = bs_0110->clone() });

    SUBCASE("group_recv ==") {
        CHECK(*di0_011 == *di0_011);
        auto di0_011_clone = std::make_unique<group_recv>(
            group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_011->clone() });
        CHECK(*di0_011 == *di0_011_clone); // check that == is not comparing addresses
        CHECK(*di0_011 != *di0_110);
        CHECK(*di0_011 != *di1_0110);
    }

    SUBCASE("group_recv ordering") {
        // same elements, different initial order
        auto left_set = std::vector<group_recv*>{ di0_011.get(), di0_110.get(), di1_0110.get() };
        auto right_set = std::vector<group_recv*>{ di0_110.get(), di1_0110.get(), di0_011.get() };

        // duplicate elements
        auto dupe_set = std::vector<group_recv*>{ di0_011.get() /*1*/, di0_011.get() /*2*/, di1_0110.get() };
        auto dupe_set_2 = std::vector<group_recv*>{ di0_011.get() /*1*/, di1_0110.get(), di0_011.get() /*2*/};

        REQUIRE(left_set != right_set);
        REQUIRE(left_set != dupe_set);
        REQUIRE(left_set != dupe_set_2);
        REQUIRE(right_set != dupe_set);
        REQUIRE(right_set != dupe_set_2);
        REQUIRE(dupe_set != dupe_set_2);

        const auto deref = [](const auto* ptr) -> const auto& { return *ptr; };

        std::ranges::sort(left_set, {}, deref);
        std::ranges::sort(right_set, {}, deref);
        std::ranges::sort(dupe_set, {}, deref);
        std::ranges::sort(dupe_set_2, {}, deref);

        CHECK(std::ranges::equal(left_set, right_set, {}, deref, deref));
        CHECK(std::ranges::equal(dupe_set, dupe_set_2, {}, deref, deref));
    }

    SUBCASE("rec_set ==") {
        auto left_rset = rec_set_t();
        auto right_rset = rec_set_t();

        // addReceiver takes ownership of the passed group, so we make some clones
        auto di0_011_clone = std::make_unique<group_recv>(
            group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_011->clone() });
        auto di0_110_clone = std::make_unique<group_recv>(
            group_recv{ .rec_di = *dih0.lock(), .rec_group = bs_110->clone() });
        auto di1_0110_clone = std::make_unique<group_recv>(
            group_recv{ .rec_di = *dih1.lock(), .rec_group = bs_0110->clone() });

        REQUIRE(left_rset == right_rset);

        SUBCASE("1") {
            left_rset.addReceiver(di0_011.release());
            right_rset.addReceiver(di0_110_clone.release());
            REQUIRE(left_rset != right_rset);

            // both sets will merge the passed bitsets into single di0_111
            left_rset.addReceiver(di0_110.release());
            right_rset.addReceiver(di0_011_clone.release());
            REQUIRE(left_rset == right_rset);

            left_rset.addReceiver(di1_0110.release());
            REQUIRE(left_rset != right_rset);

            right_rset.addReceiver(di1_0110_clone.release());
            REQUIRE(left_rset == right_rset);
        }

        SUBCASE("2") {
            left_rset.addReceiver(di0_011.release());
            left_rset.addReceiver(di1_0110.release());
            REQUIRE(left_rset != right_rset);

            right_rset.addReceiver(di0_011_clone.release());
            REQUIRE(left_rset != right_rset);

            right_rset.addReceiver(di1_0110_clone.release());
            REQUIRE(left_rset == right_rset);

            left_rset.addReceiver(di0_110.release());
            REQUIRE(left_rset != right_rset);

            right_rset.addReceiver(di0_110_clone.release());
            REQUIRE(left_rset == right_rset);
        }

        SUBCASE("3") {
            // verify that order doesn't matter
            left_rset.addReceiver(di0_011.release());
            left_rset.addReceiver(di1_0110.release());
            right_rset.addReceiver(di1_0110_clone.release());
            right_rset.addReceiver(di0_011_clone.release());
            REQUIRE(left_rset == right_rset);
        }
    }

    // clean up as to not affect other tests
    DbgInvoke::deactivateAll();
}

// Public API for accessing dbginvoke table

namespace {

// private table of all active dbginvokes
std::vector<std::shared_ptr<DbgInvoke>> g_allInvokes;

// cached list of dbginvoke handles to return via getAllDbgInvokes()
// to avoid rebuilding the list in every call to DbgCheckEvents etc.
std::vector<DbgInvoke::Handle> g_allHandles;
void rebuildCachedHandles() {
    g_allHandles.clear();
    for (const auto& di : g_allInvokes) {
        g_allHandles.emplace_back(di->handle());
    }
}

// Automatically join thread in destructor to allow collection of
// cleanup threads from expiring DbgInvoke objects
std::vector<DbgInvoke::WaitableThread> g_allThreads;

} // anonymous namespace

DbgInvoke::Handle DbgInvoke::activate(std::string name, std::unique_ptr<DbgInvoke>&& di) {
    if (!di || name == "") return nullptr;

    // disallow duplicate names
    for (const auto& existingDi : g_allInvokes) {
        if (name == existingDi->handle().name()) {
            return nullptr;
        }
    }

    auto sptr = std::shared_ptr<DbgInvoke>(std::move(di));
    auto handle = DbgInvoke::Handle(name, sptr);

    sptr->m_handle = handle;

    g_allInvokes.push_back(std::move(sptr));
    g_allHandles.push_back(handle);

    return handle;
}

bool DbgInvoke::deactivate(const DbgInvoke::Handle& handle) {
    // find it
    auto it = std::find_if(g_allInvokes.begin(), g_allInvokes.end(), [&handle](const auto& diptr) {
        return handle == diptr->handle();
    });

    if (it == g_allInvokes.end()) return false;

    // remove it
    auto expiring = std::move(*it);
    g_allInvokes.erase(it);

    rebuildCachedHandles();

    return true;
}

DbgInvoke::Handle DbgInvoke::rename(DbgInvoke::Handle handle, std::string newName) {
    if (!handle.lock() || newName.empty()) return nullptr;

    for (auto& existingDi : g_allInvokes) {
        if (existingDi->handle().name() == handle.name()) {
            existingDi->m_handle = DbgInvoke::Handle(newName, existingDi);
            rebuildCachedHandles();
            return existingDi->handle();
        }
    }

    return nullptr;
}

void DbgInvoke::deactivateAll() {
    g_allInvokes.clear();
    rebuildCachedHandles();
}

std::vector<DbgInvoke::Handle>& DbgInvoke::getAll() {
    return g_allHandles;
}

DbgInvoke::DbgInvoke(ProcessType type, int nproc, DebugClientProtocol& proto)
    : m_type{type}
    , m_nproc{nproc}
    , m_proto{proto}
{};

DbgInvoke::~DbgInvoke() {
    HoldSignals();
    ReleaseSignals();

    // Disown exit thread for later collection
    if (m_exitThread) {
        g_allThreads.emplace_back(std::move(*m_exitThread));
    }
}

bool DbgInvoke::operator==(const DbgInvoke& other) const {
    return m_handle == other.m_handle;
}

bool DbgInvoke::operator!=(const DbgInvoke& other) const {
    return !(*this == other);
}

DbgInvoke::Handle DbgInvoke::handle() {
    if (m_handle.lock() == nullptr) {
        // The handle is put in place by DbgInvoke::activate. Don't call handle() before that.
        throw std::runtime_error("Bad dbginvoke handle");
    }
    return m_handle;
}

const DbgInvoke::Handle DbgInvoke::handle() const {
    // OK as long as handle() never gains any side effects
    return const_cast<DbgInvoke*>(this)->handle();
}

uint64_t DbgInvoke::appId() const { return m_appid; }
void DbgInvoke::setAppId(uint64_t id) { m_appid = id; }

DbgInvoke::ProcessType DbgInvoke::processType() const { return m_type; }

bool DbgInvoke::isStarted() const { return m_started_flag; }
void DbgInvoke::setStarted() { m_started_flag = true; }

bool DbgInvoke::isAtInitialBreakpoint() const { return m_at_init_breakpoint_flag; }
void DbgInvoke::setAtInitialBreakpoint(bool at) { m_at_init_breakpoint_flag = at; }

bool DbgInvoke::isShutdown() const { return m_shutdown_flag; }
void DbgInvoke::setShutdown() { m_shutdown_flag = true; }

int DbgInvoke::numRanks() const { return m_nproc; }
void DbgInvoke::setNumRanks(int n) { m_nproc = n; }

const std::vector<int>& DbgInvoke::eventFds() const { return m_evfds; }
void DbgInvoke::addEventFd(int fd) { m_evfds.push_back(fd); }
void DbgInvoke::initializeEventFds() {
    m_evfds.clear();
}

const Bitset* DbgInvoke::fullBitset() const { return m_bitset_full.get(); }
void DbgInvoke::setFullBitset(std::unique_ptr<Bitset>&& bitset) {
    m_bitset_full = std::move(bitset);
}

const Bitset* DbgInvoke::launchedBitset() const { return m_bitset_launched.get(); }
void DbgInvoke::setLaunchedBitset(std::unique_ptr<Bitset>&& bitset) {
    m_bitset_launched = std::move(bitset);
}
void DbgInvoke::orLaunchedBitset(const Bitset& rhs) {
    if (!m_bitset_launched) return;
    *m_bitset_launched |= rhs;
}

g_progoutp_t* DbgInvoke::protoOutput() { return m_proto_outp; }
void DbgInvoke::initializeProtoOutput() {
    destroyProtoOutput();
    m_proto_outp = DbgCreateGenericProgOutp();
}
void DbgInvoke::destroyProtoOutput() {
    DbgFreeGenericProgOutp(m_proto_outp);
    m_proto_outp = nullptr;
}

const LaunchOpts& DbgInvoke::launchOptions() const { return m_launch_opts; }
LaunchOpts& DbgInvoke::launchOptions() { return m_launch_opts; }

DebugClientProtocol& DbgInvoke::proto() { return m_proto; }

DebugClientNetwork* DbgInvoke::network() const { return m_proto_net.get(); }

void DbgInvoke::setNetwork(std::unique_ptr<DebugClientNetwork>&& net) {
    m_proto_net = std::move(net);
}

std::unique_ptr<DebugClientNetwork> DbgInvoke::ejectNetwork() {
    return std::move(m_proto_net);
}

void DbgInvoke::adoptExitThread(DbgInvoke::WaitableThread&& wt){
    m_exitThread = std::move(wt);
}

DbgFE* DbgInvoke::instantiateFrontendGdb(const char* gdbPath) {
    m_frontend_gdb.reset(new DbgFE(gdbPath));
    return m_frontend_gdb.get();
}
void DbgInvoke::resetFrontendGdb() { m_frontend_gdb.reset(); }

DbgFE* DbgInvoke::frontendGdb() const { return m_frontend_gdb.get(); }

void DbgInvoke::clearFrontendGdb() {
    if (m_frontend_gdb) m_frontend_gdb->clearListState();
}

void DbgInvoke::initCachedFrames() {
    if (m_bitset_full) {
        m_recent_frames = std::make_unique<BitsetMRUCache<dbgloc_t>>(*m_bitset_full);
    }
}

const BitsetMRUCache<dbgloc_t>* DbgInvoke::getCachedFrames() const {
    return m_recent_frames.get();
}

void DbgInvoke::cacheFrame(std::unique_ptr<Bitset>&& ranks, dbgloc_t&& frame) {
    if (!m_recent_frames) return;
    m_recent_frames->push(std::move(ranks), std::move(frame));
}

DbgInvoke::AsyncEventCallback DbgInvoke::getAsyncEventCallback() const {
    return m_clnt_ev_callback;
}

void DbgInvoke::setAsyncEventCallback(DbgInvoke::AsyncEventCallback func) {
    m_clnt_ev_callback = func;
}

int DbgInvoke::callAsyncEventCallback(DbgInvoke::EventSet& es, const Bitset& bitset) const {
    return m_clnt_ev_callback(es, bitset);
}

DbgInvoke::SourcePaths& DbgInvoke::sourcePaths() { return m_source_paths; }
const DbgInvoke::SourcePaths& DbgInvoke::sourcePaths() const { return m_source_paths; }

BreakpointManager& DbgInvoke::breakpoints() { return m_breakpoints; }
const BreakpointManager& DbgInvoke::breakpoints() const { return m_breakpoints; }

CheckpointManager& DbgInvoke::checkpoints() { return m_checkpoints; }
const CheckpointManager& DbgInvoke::checkpoints() const { return m_checkpoints; }

std::list<DbgInvoke::EventSet>& DbgInvoke::eventSets() { return m_eventset_list; }

DbgInvoke::Handle::Handle(std::string name, std::shared_ptr<DbgInvoke> parent) {
    if (parent == nullptr) {
        // use ::Handle(nullptr_t)
        throw std::runtime_error("Can't construct named DbgInvoke::Handle from nullptr");
    }

    m_name = std::move(name);
    m_ptr = parent;
}

DbgInvoke::Handle::Handle(nullptr_t) {
    // Creates a null handle. lock() will return nullptr
}

const std::string& DbgInvoke::Handle::name() const {
    if (lock()) {
        // If this handle is still valid, fetch name from parent.
        // It might have changed via DbgInvoke::rename.
        m_name = lock()->handle().m_name;
    }
    return m_name;
}

const char* DbgInvoke::Handle::c_str() const {
    return name().c_str();
}

DbgInvoke* DbgInvoke::Handle::lock() const {
    return m_ptr.lock().get();
}

// Parse a section of MPMD arguments
// Update `opts` with any parsed global flags,
// Return a JobOpt object with parsed job options
static auto parse_argv_section(LaunchOpts& opts, int argc, char** argv) {

    const auto withoutQuotes = [](const std::string& str) {
        const auto quotes = {'"', '\''};
        if (str.size() > 1) {
            for (const auto& q : quotes) {
                if (str.front() == q && str.back() == q) {
                    return std::string_view(str.c_str() + 1, str.size() - 2);
                }
            }
        }

        return std::string_view{str};
    };

    int parsed_gpu_flag = -1;
    int parsed_debug_flag = -1;
    int parsed_non_mpi_flag = 0;
    bool manually_specified_python_flag = false;

    std::optional<std::string> parsed_debug_dir;

    LaunchOpts::JobOpt job_opt;
    job_opt.app_argv << "";      // reserve binary name

    const struct option launch_long_opts[] = {
        {"args", required_argument, 0, 'a'},
        {"launcher-args", required_argument, 0, 'g'},
        {"launcher-input", required_argument, 0, 'i'},
        {"workdir", optional_argument, 0, 'd'},
        {"env", required_argument, 0, 'e'},
        {"gpu", no_argument, &parsed_gpu_flag, 1},
        {"debug", optional_argument, 0, 'D'},
        {"qsub", required_argument, 0, 'q'},
        {"sbatch", required_argument, 0, 's'},
        {"non-mpi", no_argument, &parsed_non_mpi_flag, 1},
        {"launcher", required_argument, 0, 'l'},
        {"gdb", required_argument, 0, 'b'},
        {"gdb-data-dir", required_argument, 0, 'B'},
        {"python", no_argument, 0, 'p'},
        {"no-python", no_argument, 0, 'P'},
        {0, 0, 0, 0}
    };

    int opt;
    /* extern int */ optind = 0;

    // This will permute the non-option stuff to the end, so parse that stuff last.
    while ((opt = getopt_long(argc, argv, "a:g:i:d::b:l:e:q:s:pP", launch_long_opts, nullptr)) != -1) {
        switch (opt) {
            case 0:
                // if this is a flag, do nothing
                break;

            case 'a':
                if (optarg == nullptr) {
                    throw std::runtime_error("--args provided, but missing arguments.");
                }
                job_opt.app_argv.parseAndAppendArgs(withoutQuotes(optarg));

                // remove quotes from individual args
                for (size_t i = 0; i < job_opt.app_argv.argc(); i++) {
                    job_opt.app_argv[i] = withoutQuotes(job_opt.app_argv[i]);
                }

                break;

            case 'l':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher provided, but missing arguments.");
                }

                // replace on first instance of --launcher only
                if (opts.launcher_name.empty()) {
                    opts.launcher_name = withoutQuotes(optarg);
                }
                break;

            case 'g':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher-args provided, but missing arguments.");
                }

                job_opt.launcher_args.parseAndAppendArgs(withoutQuotes(optarg));

                // remove quotes from individual args
                for (size_t i = 0; i < job_opt.launcher_args.argc(); i++) {
                    job_opt.launcher_args[i] = withoutQuotes(job_opt.launcher_args[i]);
                }
                break;

            case 'i':
                if (optarg == nullptr) {
                    throw std::runtime_error("--launcher-input provided, but missing arguments.");
                }
                opts.input_file_path = withoutQuotes(optarg);
                break;

            case 'd':
                if (optarg == nullptr) {
                    // --workdir provided without argument, so default to chdir
                    // to the directory that the binary is located in.
                    opts.set_default_cwd = true;
                } else {
                    opts.cwd = withoutQuotes(optarg);
                }
                break;

            case 'e': {
                if (optarg == nullptr) {
                    throw std::runtime_error("--env provided, but missing arguments.");
                }

                // Check formatting
                auto view = std::string_view{optarg};
                if (view.size() < 3 || std::count(view.begin(), view.end(), '=') != 1){
                    using namespace std::string_literals;
                    throw std::runtime_error("Invalid --env string\""s + optarg
                        + "\". Expected \"KEY=VALUE\"");
                }

                opts.env << optarg;
                break;
            }

            case 'D':
                parsed_debug_flag = 1;
                if (optarg && *optarg) parsed_debug_dir = optarg;
                break;

            case 'b':
                if (optarg == nullptr) {
                    throw std::runtime_error("--gdb provided, but missing argument.");
                }
                opts.gdb_path = withoutQuotes(optarg);
                break;

            case 'B':
                if (std::filesystem::is_directory(optarg)) {
                    opts.gdb_data_dir = optarg;
                } else {
                    throw std::runtime_error("--gdb-data-dir is not a directory.");
                }
                break;
                    

            case 'q':
                if (optarg == nullptr) {
                    throw std::runtime_error("--qsub provided, but missing arguments.");
                }
                opts.qsub_file_path = withoutQuotes(optarg);
                break;

            case 's':
                if (optarg == nullptr) {
                    throw std::runtime_error("--sbatch provided, but missing arguments.");
                }
                opts.sbatch_file_path = withoutQuotes(optarg);
                break;

            case 'p':
                job_opt.python = true;
                manually_specified_python_flag = true;
                break;

            case 'P':
                job_opt.python = false;
                manually_specified_python_flag = true;
                break;

            default:
                throw std::runtime_error("Invalid launch argument provided. See \"help launch\" for usage.");
        }
    }

    const auto getPositionalArgs = [&](bool needs_procset) {
        // the binary name + the procset name
        const auto required = 1 + static_cast<int>(needs_procset);
        const auto provided = argc - optind;

        if (provided < required) {
            using namespace std::string_literals;
            throw std::runtime_error(
                "Invalid number of non-option arguments. See \"help launch\" for usage.\n"s
                "Expected at least " + std::to_string(required) + " (" + (required > 1 ? "process set name and " : "") + "application binary)."
            );
        }

        // The first argument should be the name of the process set if needed
        if (needs_procset) {
            if (auto procset_name_cstr = argv[optind++]) {
                opts.procset_name = procset_name_cstr;
            } else {
                throw std::runtime_error("No process set specified");
            }
        }

        // The second argument is the name of the a.out (always needed)
        if (auto binary_path_cstr = argv[optind++]) {
            job_opt.app_argv[0] = binary_path_cstr;
        } else {
            throw std::runtime_error("Invalid binary (No binary passed)");
        }

        // The rest are application arguments as long as --args wasn't already used.
        if ((provided > required) && (job_opt.app_argv.argc() > 1)) {
            throw std::runtime_error(
                "Both --args=\"...\" and positional arguments were used. Use one or the other."
            );
        }

        for (; optind < argc; optind++) {
            job_opt.app_argv << withoutQuotes(argv[optind]);
        }
    };

    // in an mpmd launch, we parse multiple sets of launch arguments for a single procset.
    // we might already have parsed the procset name from a previous set of arguments.
    getPositionalArgs(opts.procset_name.empty());

    if (opts.set_default_cwd) {
        // --workdir provided without argument, so default to chdir to
        // the directory that the binary is located in.
        auto binary_path = std::filesystem::path{job_opt.app_argv[0]};
        opts.cwd = binary_path.parent_path();
    }

    // Set flags if updated
    if (parsed_gpu_flag >= 0) {
        opts.gpu_flag = parsed_gpu_flag;
    }
    if (parsed_debug_flag >= 0) {
        opts.debug_flag = parsed_debug_flag;
    }
    if (parsed_debug_dir) {
        if (!DbgSetLogDir(*parsed_debug_dir)) {
            throw std::runtime_error("--debug must be a directory writeable by the user");
        }
    }

    // auto detect python if a manual option wasn't specified
    // TODO: make this better
    if (!manually_specified_python_flag && job_opt.app_argv.argc() > 0) {
        job_opt.python = job_opt.app_argv[0].find("python3") != std::string::npos;
    }

    // Update job options
    job_opt.non_mpi_flag = parsed_non_mpi_flag;
    job_opt.run_argv = job_opt.app_argv;

    return job_opt;
}

LaunchOpts::LaunchOpts(int argc, const char* const constArgv[], ClientSettingsInterface& csi) {
    if (argc < 3) {
        throw std::runtime_error("Not enough command arguments.\nExpected: launch $handle ./program <args>");
    }

    // getopt can change argv so we work with a local copy
    struct CStrVector {
        std::vector<char*> v;
        // ensure memory is cleaned up if something throws
        CStrVector() : v{} {}
        ~CStrVector() {
            for (auto& str : v) { if (str) { ::free(str); } }
        }
        CStrVector(CStrVector&& expiring) : v{std::move(expiring.v)} {
            for (auto&& str : expiring.v) { str = nullptr; }
        }
        CStrVector(CStrVector&) = delete;
        CStrVector(CStrVector const&) = delete;
    };

    // Break up MPMD launch parameters delimited by :
    std::vector<CStrVector> argv_sections;
    argv_sections.push_back(CStrVector());
    for (int i = 0; i < argc; i++) {
        if ((constArgv[i][0] == ':') && (constArgv[i][1] == '\0')) {
            argv_sections.push_back(CStrVector());
            argv_sections.back().v.push_back(::strdup(":"));
        } else {
            argv_sections.back().v.push_back(::strdup(constArgv[i]));
        }
    }

    for (auto&& argv_section : argv_sections) {
        auto job_opt = parse_argv_section(*this, argv_section.v.size(), argv_section.v.data());
        job_opts.push_back(std::move(job_opt));
    }

    non_stop_flag = csi.isNonStop();
    early_entry_flag = csi.isEarlyEntry();
    unsafe_flag = csi.isUnsafe();
    proto_name = csi.getProto();
    backend_name = csi.getBackend();
}

void LaunchOpts::enforceValid() {
    namespace fs = std::filesystem;
    using namespace std::string_literals;

    const auto fTypeStr = [](fs::file_type t) {
        std::string fType = "";
        switch(t) {
            case fs::file_type::none: fType = "is not evaluated"; break;
            case fs::file_type::not_found: fType = "does not exist"; break;
            case fs::file_type::regular: fType = "is a regular file"; break;
            case fs::file_type::directory: fType = "is a directory"; break;
            case fs::file_type::symlink: fType = "is a symlink"; break;
            case fs::file_type::block: fType = "is a block device"; break;
            case fs::file_type::character: fType = "is a character device"; break;
            case fs::file_type::fifo: fType = "is a named IPC pipe"; break;
            case fs::file_type::socket: fType = "is a named IPC socket"; break;
            case fs::file_type::unknown: fType = "has `unknown` type"; break;
        }
        return fType;
    };

    const auto canonFile = [fTypeStr](std::string& path) {
        if (!path.empty()) {
            // throws if path doesn't exist
            auto canon_path = fs::canonical(path);
            auto status = fs::status(canon_path); // follows links
            if (!fs::is_regular_file(status)) {
                throw std::runtime_error(
                    canon_path.string() + " is not a regular file, it " + fTypeStr(status.type())
                );
            }
            path = canon_path;
        }
    };

    for (auto&& job_opt : job_opts) {
        // find argv[0] in PATH so it's easy for users to launch python jobs e.g.
        // launch $a{3} python3 -a "my_app.py"
        if (auto in_path = MallocedCharPtr(pathFind(job_opt.app_argv[0].c_str(), "PATH"))) {
            job_opt.app_argv[0] = in_path.get();
        }
        if (auto in_path = MallocedCharPtr(pathFind(job_opt.run_argv[0].c_str(), "PATH"))) {
            job_opt.run_argv[0] = in_path.get();
        }

        canonFile(job_opt.app_argv[0]);
        canonFile(job_opt.run_argv[0]);
    }
    canonFile(launcher_name);
    canonFile(input_file_path);
    canonFile(qsub_file_path);
    canonFile(sbatch_file_path);

    if (!cwd.empty()) {
        // throws if cwd doesn't exist
        auto canon_cwd = fs::canonical(cwd);
        auto status = fs::status(canon_cwd); // follows links
        if (!fs::is_directory(status)) {
            throw std::runtime_error(
                canon_cwd.string() + " is not a directory, it " + fTypeStr(status.type())
            );
        }
        cwd = canon_cwd;
    }

    if (!gdb_path.empty()) {
        // the user can specify this option as --gdb=/path/to/gdb:/path/to/gdb/datadir or
        // just --gdb=/path/to/gdb
        auto colon_idx = gdb_path.find(":");

        auto gdb_binary = gdb_path.substr(0, colon_idx);
        auto data_dir = std::string{""};
        if (colon_idx != std::string::npos) {
            data_dir = gdb_path.substr(colon_idx + 1);
        }

        // check gdb path
        if (auto p = pathFind(gdb_binary.c_str(), nullptr)) {
            namespace fs = std::filesystem;
            // make sure p is cleaned up in case fs:canonical throws
            auto p_str = std::string{p};
            free(p);
            gdb_binary = fs::canonical(p_str);
            canonFile(gdb_binary);
        } else {
            using namespace std::string_literals;
            throw std::runtime_error(
                "Could not find --gdb provided ("s + gdb_path + ") in PATH."
            );
        }

        // check datadir path if present
        if (!data_dir.empty()) {
            // throws if data_dir doesn't exist
            auto canon_data_dir = fs::canonical(data_dir);
            auto status = fs::status(canon_data_dir); // follows links
            if (!fs::is_directory(status)) {
                throw std::runtime_error(
                    canon_data_dir.string() + " is not a directory, it " + fTypeStr(status.type())
                );
            }
            data_dir = canon_data_dir;
        }

        // compose full path string
        gdb_path = gdb_binary;
        if (!data_dir.empty()) gdb_path += ":" + data_dir;
    }
}

void DbgInvoke::SourcePaths::storeSourcePath(const char * str) {
    if(str!=nullptr){
        m_sourceList.push_back(std::filesystem::path(str));
    }
}

std::string DbgInvoke::SourcePaths::getFullPath(const std::string& relativePath) const {
    if (!relativePath.empty() && !m_sourceList.empty()){
        for(auto &source : m_sourceList){
            if (source.find(relativePath) != std::string::npos){
                return source;
            }
        }
    }
    
    //Cannot find full path. Return the relative path as the full path.
    return relativePath;
}

DbgInvoke::EventSet::EventSet(DbgInvoke& di, const Bitset& group, const dbgevent_t* event)
    : m_di{di}, m_group{group.clone()}, m_event{DupEvent(event)}
{}

DbgInvoke::EventSet::~EventSet() {
    if (m_event) FreeEvent(m_event);
}

DbgInvoke::WaitableThread::WaitableThread(std::thread&& t, std::shared_ptr<std::mutex> mtx,
    std::shared_ptr<std::condition_variable> cv, std::shared_ptr<bool> joinable)
    : m_thread{std::move(t)}
    , m_mtx{std::move(mtx)}
    , m_cv{std::move(cv)}
    , m_joinable{std::move(joinable)}
    , m_threadValid{true}
{}

DbgInvoke::WaitableThread::WaitableThread(WaitableThread&& expiring)
    : m_thread{std::move(expiring.m_thread)}
    , m_mtx{std::move(expiring.m_mtx)}
    , m_cv{std::move(expiring.m_cv)}
    , m_joinable{std::move(expiring.m_joinable)}
    , m_threadValid{expiring.m_threadValid}
{
    expiring.m_threadValid = false;
}

DbgInvoke::WaitableThread& DbgInvoke::WaitableThread::operator= (WaitableThread&& rhs) {
    m_thread = std::move(rhs.m_thread);
    m_mtx = std::move(rhs.m_mtx);
    m_cv = std::move(rhs.m_cv);
    m_joinable = std::move(rhs.m_joinable);
    m_threadValid = rhs.m_threadValid;

    rhs.m_threadValid = false;

    return *this;
}

DbgInvoke::WaitableThread::~WaitableThread()
{
    if (!m_threadValid) {
        return;
    }

    try {
        auto lck = std::unique_lock<std::mutex>{*m_mtx};

        // Wait for cleanup thread to complete
        if (m_cv->wait_for(lck, std::chrono::seconds(10),
            [this]{ return *m_joinable; })) {

            m_thread.join();

        // Timed out, detach thread
        } else {
            PrintOutMsg("warning: cleanup timed out, your job may still be running\n");
            m_thread.detach();
        }

    } catch (...) {
        // Ignore exception
    }
}
