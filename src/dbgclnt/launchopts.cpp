// Copyright 2023-2024 Hewlett Packard Enterprise Development LP.

#include <getopt.h>

#include <filesystem>
#include <fstream>
#include <optional>

#include "client/cli_funcs.h"
#include "unittest_macros.h"

#include "path.h"
#include "str.h"

#include "launchopts.h"

namespace test {

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

} // namespace test

TEST_CASE("Parse LaunchOpts") {
    SUBCASE("Throws exception when missing required args") {
        const char* const argv[] = {};
        auto csi = test::MockClientSettings();

        CHECK_THROWS(LaunchOpts::parse_gdb4hpc(0, argv, csi));
    }

    SUBCASE("Settings from global client settings") {
        const char* const argv[] = {
            "launch", "$a{1}", "/tmp/c_inc",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

        CHECK(opts.unsafe_flag == false);
        CHECK(opts.non_stop_flag == true);
        CHECK(opts.early_entry_flag == true);
        CHECK(opts.proto_name == "mock_zmqnet");
        CHECK(opts.backend_name == "mock_gdbmi");
    }

    SUBCASE("Path settings: gdb4hpc: unquoted") {
        const char* const argv[] = {
            "launch", "$a{1}", "./bin/c_inc",

            "--launcher=/tmp/fake_mpiexec",
            "--launcher-input=/tmp/fake_input",
            "--gdb=/tmp/fake_gdb",
            "--workdir=/tmp/fake_workdir",
            "--qsub=/tmp/fake_qsub",
            "--sbatch=/tmp/fake_sbatch",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.job_opts.size() == 1);
        CHECK(opts.launcher_name == "/tmp/fake_mpiexec");
        CHECK(opts.input_file_path == "/tmp/fake_input");
        CHECK(opts.gdb_path == "/tmp/fake_gdb");
        CHECK(opts.cwd == "/tmp/fake_workdir");
        CHECK(opts.qsub_file_path == "/tmp/fake_qsub");
        CHECK(opts.sbatch_file_path == "/tmp/fake_sbatch");
    }

    SUBCASE("Path settings: gdb4hpc: quoted") {
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
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
            auto csi = test::MockClientSettings();
            auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
            auto csi = test::MockClientSettings();
            auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
            auto csi = test::MockClientSettings();
            auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
            auto csi = test::MockClientSettings();
            CHECK_THROWS(LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi));
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
            auto csi = test::MockClientSettings();
            auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
            auto csi = test::MockClientSettings();
            auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
            auto csi = test::MockClientSettings();
            auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
            auto csi = test::MockClientSettings();
            auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
            auto csi = test::MockClientSettings();
            CHECK_THROWS(LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi));
        }
    }

    SUBCASE("python") {
        SUBCASE("automatically detect python3") {
            SUBCASE ("python3") {
                const char* const argv[] = {
                    "launch", "$a{1}", "python3",
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            SUBCASE ("python3.11") {
                const char* const argv[] = {
                    "launch", "$a{1}", "python3.11",
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            SUBCASE ("/usr/bin/python3") {
                const char* const argv[] = {
                    "launch", "$a{1}", "/usr/bin/python3",
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            SUBCASE ("cray python") {
                const char* const argv[] = {
                    "launch", "$a{1}", "/global/opt/cray/pe/python/3.11.7/bin/python3.11",
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
        }

        SUBCASE("manual --python flag") {
            {   // ensure we aren't detecting python3 when it isn't there
                const char* const argv[] = {
                    "launch", "$a{1}", "./i-promise-this-is-pyth0n.x"
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == false);
            }
            {   // i promise it's there!
                const char* const argv[] = {
                    "launch", "$a{1}", "./i-promise-this-is-pyth0n.x", "--python"
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            {   // different order
                const char* const argv[] = {
                    "launch", "$a{1}", "--python", "./i-promise-this-is-pyth0n.x",
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
            {   // short form
                const char* const argv[] = {
                    "launch", "$a{1}", "./i-promise-this-is-pyth0n.x", "-p"
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == true);
            }
        }

        SUBCASE("manual --no-python flag") {
            {
                const char* const argv[] = {
                    "launch", "$a{1}", "python3", "--no-python",
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == false);
            }
            {   // different order
                const char* const argv[] = {
                    "launch", "$a{1}", "--no-python", "python3",
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == false);
            }
            {   // short form
                const char* const argv[] = {
                    "launch", "$a{1}", "-P", "python3",
                };
                auto csi = test::MockClientSettings();
                auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE(opts.job_opts.size() == 1);
                CHECK(opts.job_opts.front().python == false);
            }
        }
    }

    SUBCASE("--debug=logdir implies --debug") {
        const char* const argv[] = {
            "launch", "$a{1}", "app", "--debug=logdir",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

        CHECK(opts.debug_flag);
        CHECK(!opts.debug_dir.empty());
    }
}

TEST_CASE("LaunchArgs enforceValid() validates that files are present") {
    if (auto fs = test::MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--workdir", "--gdb=unittest_gdb",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
    if (auto fs = test::MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",

            "--launcher=./mockfs/unittest_binary",
            "--launcher-input=./mockfs/unittest_txt",
            "--gdb=./mockfs/unittest_gdb",
            "--workdir=./mockfs",
            "--qsub=./mockfs/unittest_txt",
            "--sbatch=./mockfs/unittest_txt",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
    if (auto fs = test::MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--gdb=./mockfs/unittest_gdb:./mockfs",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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
    if (auto fs = test::MockFs::make()) {
        const char* const argv[] = {
            "launch", "$a{1}", "./mockfs/unittest_binary",
            "--gdb=./mockfs/unittest_gdb:",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_gdb4hpc(sizeof(argv)/sizeof(argv[0]), argv, csi);

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

TEST_CASE("LaunchOpts: parse_srun gauntlet") {
    SUBCASE("respects client settings") {
        const char* const argv[] = {
            "srun", "-n1", "/tmp/c_inc",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);

        CHECK(opts.unsafe_flag == false);
        CHECK(opts.non_stop_flag == true);
        CHECK(opts.early_entry_flag == true);
        CHECK(opts.proto_name == "mock_zmqnet");
        CHECK(opts.backend_name == "mock_gdbmi");
    }

    SUBCASE("parse program names") {
        const char* const argv[] = {
            "srun", "./bin/c_inc.x",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.job_opts.size() == 1);

        // we intentionally don't set launcher_name because it's not necessary.
        // see srun parser implementation for more info.
        CHECK(opts.launcher_name == "");
        // in srun, default amount of ranks is 1 per node
        CHECK(opts.procset_name == "$c_inc_x{1}");
        REQUIRE(opts.job_opts[0].app_argv.argc() > 0);
        CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc.x");
    }

    SUBCASE("calculate ranks") {
        const auto check_ranks = [](const std::vector<const char*>& args, int desired) {
            const auto join = [](const auto& args) {
                auto result = std::string{};
                for (const auto& arg : args) {
                    result += arg;
                    result += " ";
                }
                return result;
            };

            CAPTURE(join(args));

            struct CStrVector {
                ~CStrVector() { for (auto& str : v) free(str); }
                std::vector<char*> v;
            };

            auto real_argv = CStrVector{};
            real_argv.v.push_back(::strdup("srun"));
            for (const auto* cstr : args) real_argv.v.push_back(::strdup(cstr));
            real_argv.v.push_back(::strdup("exe"));

            auto csi = test::MockClientSettings();
            auto opts = LaunchOpts::parse_srun(real_argv.v.size(), real_argv.v.data(), csi);

            CHECK(opts.procset_name == "$exe{" + std::to_string(desired) + "}");
        };

        // separate subcases will make the above CAPTURE make more sense in reports
        SUBCASE("") { check_ranks({"-n3"}, 3); }
        SUBCASE("") { check_ranks({"-n", "4"}, 4); }
        SUBCASE("") { check_ranks({"--ntasks", "4"}, 4); }
        SUBCASE("") { check_ranks({"--ntasks=4"}, 4); }
        SUBCASE("") { check_ranks({"-N5"}, 5); }
        SUBCASE("") { check_ranks({"-N", "6"}, 6); }
        SUBCASE("") { check_ranks({"--nodes", "6"}, 6); }
        SUBCASE("") { check_ranks({"--nodes=6"}, 6); }
        SUBCASE("") { check_ranks({"--ntasks-per-node=7"}, 7); }
        SUBCASE("") { check_ranks({"--ntasks-per-node", "8"}, 8); }

        // combos
        SUBCASE("") { check_ranks({"-n3", "-N3"}, 3); }
        SUBCASE("") { check_ranks({"-n3", "--ntasks-per-node=3"}, 3); }
        SUBCASE("") { check_ranks({"--ntasks-per-node=3", "-N3"}, 9); }
        SUBCASE("") { check_ranks({"-n3", "-N3", "--ntasks-per-node=3"}, 3); }

        // when all three are given, --ntasks overwrites everything
        SUBCASE("") { check_ranks({"-n8", "-N9", "--ntasks-per-node=3"}, 8); }
        SUBCASE("") { check_ranks({"-n4", "-N99", "--ntasks-per-node=3"}, 4); }
        SUBCASE("") { check_ranks({"-n2", "-N3", "--ntasks-per-node=4"}, 2); }

        // but when only --ntasks and --ntasks per node are given, there
        // is an srun bug.
        SUBCASE("") { check_ranks({"-n3", "--ntasks-per-node=9"}, 9); }
    }

    SUBCASE("calculate ranks mpmd") {
        const char* const argv[] = {
            "srun", "-n4", "./bin/c_inc", ":", "--ntasks-per-node=4", "-N4", "./bin/c_frame",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.job_opts.size() == 2);

        // 4 ranks plus 16 ranks
        CHECK(opts.procset_name == "$c_inc{20}");
    }

    SUBCASE("launcher args") {
        const char* const argv[] = {
            "srun", "-n4", "-p", "allnodes", "--ntasks-per-node=2", "./bin/c_inc",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.job_opts.size() == 1);

        REQUIRE(opts.job_opts[0].launcher_args.argc() == 4);

        // short option names are expanded and any "--longoption=n" is broken up
        // into "--longoption n"
        CHECK(opts.job_opts[0].launcher_args[0] == "--partition");
        CHECK(opts.job_opts[0].launcher_args[1] == "allnodes");
        CHECK(opts.job_opts[0].launcher_args[2] == "--ntasks-per-node");
        CHECK(opts.job_opts[0].launcher_args[3] == "2");
    }

    SUBCASE("bad launcher args") {
        const char* const argv[] = {
            "srun", "-n", "./bin/c_inc",
        };
        auto csi = test::MockClientSettings();
        CHECK_THROWS(LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi));
    }

    SUBCASE("app and launcher args") {
        const char* const argv[] = {
            "srun", "-n4", "-p", "fastnodes", "./bin/c_inc", "3", "100", "--", "sneaky", ":",
            "--ntasks-per-node=4", "-N4", "--gres=consumable,resources", "./bin/c_frame", "999", "-b",
        };
        auto csi = test::MockClientSettings();
        auto opts = LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);

        REQUIRE(opts.job_opts.size() == 2);
        CHECK(opts.procset_name == "$c_inc{20}");

        REQUIRE(opts.job_opts[0].app_argv.argc() == 5);
        CHECK(opts.job_opts[0].app_argv[0] == "./bin/c_inc");
        CHECK(opts.job_opts[0].app_argv[1] == "3");
        CHECK(opts.job_opts[0].app_argv[2] == "100");
        CHECK(opts.job_opts[0].app_argv[3] == "--");
        CHECK(opts.job_opts[0].app_argv[4] == "sneaky");

        REQUIRE(opts.job_opts[0].launcher_args.argc() == 2);
        // we don't forward -n4
        CHECK(opts.job_opts[0].launcher_args[0] == "--partition");
        CHECK(opts.job_opts[0].launcher_args[1] == "fastnodes");

        REQUIRE(opts.job_opts[1].app_argv.argc() == 3);
        CHECK(opts.job_opts[1].app_argv[0] == "./bin/c_frame");
        CHECK(opts.job_opts[1].app_argv[1] == "999");
        CHECK(opts.job_opts[1].app_argv[2] == "-b");

        REQUIRE(opts.job_opts[1].launcher_args.argc() == 6);
        CHECK(opts.job_opts[1].launcher_args[0] == "--ntasks-per-node");
        CHECK(opts.job_opts[1].launcher_args[1] == "4");
        CHECK(opts.job_opts[1].launcher_args[2] == "--nodes");
        CHECK(opts.job_opts[1].launcher_args[3] == "4");
        CHECK(opts.job_opts[1].launcher_args[4] == "--gres");
        CHECK(opts.job_opts[1].launcher_args[5] == "consumable,resources");
    }

    SUBCASE("reports unsupported options") {
        SUBCASE("short option") {
            const char* const argv[] = {
                "srun", "-z", "./bin/c_inc",
            };
            auto csi = test::MockClientSettings();
            try {
                LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE_MESSAGE(false, "Never threw!");
            } catch (const std::runtime_error& e) {
                CAPTURE(std::string{e.what()});
                CHECK_MESSAGE(std::string_view{e.what()}.find("Unsupported srun option: -z") != std::string_view::npos, "Error message wasn't informative");
            }
        }

        SUBCASE("short option 2") {
            const char* const argv[] = {
                "srun", "-z2", "./bin/c_inc",
            };
            auto csi = test::MockClientSettings();
            try {
                LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE_MESSAGE(false, "Never threw!");
            } catch (const std::runtime_error& e) {
                CAPTURE(std::string{e.what()});
                CHECK_MESSAGE(std::string_view{e.what()}.find("Unsupported srun option: -z") != std::string_view::npos, "Error message wasn't informative");
            }
        }

        SUBCASE("long option") {
            const char* const argv[] = {
                "srun", "--gnome", "./bin/c_inc",
            };
            auto csi = test::MockClientSettings();
            try {
                LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE_MESSAGE(false, "Never threw!");
            } catch (const std::runtime_error& e) {
                CAPTURE(std::string{e.what()});
                CHECK_MESSAGE(std::string_view{e.what()}.find("Unsupported srun option: --gnome") != std::string_view::npos, "Error message wasn't informative");
            }
        }

        SUBCASE("long option 2") {
            const char* const argv[] = {
                "srun", "-n3", "-N3", "--gnome=2", "./bin/c_inc", "./my_input.txt",
            };
            auto csi = test::MockClientSettings();
            try {
                LaunchOpts::parse_srun(sizeof(argv)/sizeof(argv[0]), argv, csi);
                REQUIRE_MESSAGE(false, "Never threw!");
            } catch (const std::runtime_error& e) {
                CAPTURE(std::string{e.what()});
                CHECK_MESSAGE(std::string_view{e.what()}.find("Unsupported srun option: --gnome") != std::string_view::npos, "Error message wasn't informative");
            }
        }
    }
}

class CStrVector {
    // we use getopt, which
    // requires mutable char*s (getopt changes argv in place),
    // but the parsing functions receive const char* const[].
    // so they need to work with a local copy of char*s.
public:
    CStrVector() : m_vec{} {}
    ~CStrVector() {
        for (auto& str : m_vec) { if (str) { ::free(str); } }
    }
    CStrVector(CStrVector&& expiring) : m_vec{std::move(expiring.m_vec)} {
        for (auto&& str : expiring.m_vec) { str = nullptr; }
    }
    CStrVector(CStrVector&) = delete;
    CStrVector(CStrVector const&) = delete;

    // takes ownership of passed `cstr`
    void push_back(char* cstr) { m_vec.push_back(cstr); }

    size_t size() const { return m_vec.size(); }
    char** data() { return m_vec.data(); }

private:
    std::vector<char*> m_vec;
};

// Break up slurm style MPMD launch parameters delimited by ':'. the first
// returned section will start with "srun" and subseqent sections will start
// with ":".
static std::vector<CStrVector> get_mpmd_sections(int argc, const char* const argv[]) {
    // srun [OPTIONS(0)... [executable(0) [args(0)...]]] [ : [OPTIONS(N)...]] executable(N) [args(N)...]
    std::vector<CStrVector> argv_sections;
    argv_sections.push_back(CStrVector());
    for (int i = 0; i < argc; i++) {
        if ((argv[i][0] == ':') && (argv[i][1] == '\0')) {
            argv_sections.push_back(CStrVector());
            argv_sections.back().push_back(::strdup(":"));
        } else {
            argv_sections.back().push_back(::strdup(argv[i]));
        }
    }

    return argv_sections;
}

// Parse a section of MPMD arguments
// Update `opts` with any parsed global flags,
// Return a JobOpt object with parsed job options
static auto parse_gdb4hpc_argv_section(LaunchOpts& opts, int argc, char** argv) {
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
    int parsed_non_mpi_flag = 0;
    bool manually_specified_python_flag = false;

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

    // throws on failure
    const auto optstring = optstring_from_longopts(launch_long_opts, OptstringScanningMode::PermuteArgv, /*differentiate_missing_arg*/ false);

    // This will permute the non-option stuff to the end, so parse that stuff last.
    while ((opt = getopt_long(argc, argv, optstring.c_str(), launch_long_opts, nullptr)) != -1) {
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
                opts.debug_flag = true;
                if (optarg && *optarg) opts.debug_dir = optarg;
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

LaunchOpts LaunchOpts::parse_gdb4hpc(int argc, const char* const argv[], ClientSettingsInterface& csi) {
    auto result = LaunchOpts{};

    if (argc < 3) {
        throw std::runtime_error("Not enough command arguments.\nExpected: launch $handle ./program <args>");
    }

    for (auto&& argv_section : get_mpmd_sections(argc, argv)) {
        auto job_opt = parse_gdb4hpc_argv_section(result, argv_section.size(), argv_section.data());
        result.job_opts.push_back(std::move(job_opt));
    }

    result.read_client_settings(csi);

    return result;
}

static auto parse_srun_section(LaunchOpts& opts, int argc, char* argv[]) {
    auto job_opts = LaunchOpts::JobOpt{};

    // we can forward almost everything to launcher_args. the only real work we
    // have to do is extract a correct procset name which encodes the total
    // amount of ranks across n mpmd launches.
    //
    // however, we still need to replicate the srun command line parser as best
    // as possible. if we parse `srun -n4 -p allnodes ./my/binary`, and our
    // parser doesn't know what "-p" means, it will stop and think "allnodes" is
    // the binary name. slurm's many, many options cannot all be handled by
    // getopt because we would run out of letters to use in the switch
    // statement.
    //
    // for now, we compromise and add the options that we deem most common to
    // the parser and report an error if we encounter something we don't
    // recognize.
    //
    // this problem is tracked in PE-54906.

    const struct option options[] = {
        // all srun options with both long and short flags
        {"account", required_argument, 0, 'A'},
        {"extra-node-info", required_argument, 0, 'B'},
        {"begin", required_argument, 0, 'b'},
        {"constraint", required_argument, 0, 'C'},
        {"cpus-per-task", required_argument, 0, 'c'},
        {"chdir", required_argument, 0, 'D'},
        {"dependency", required_argument, 0, 'd'},
        {"error", required_argument, 0, 'e'},
        {"preserve-env", no_argument, 0, 'E'},
        {"gpus", required_argument, 0, 'G'},
        {"hold", no_argument, 0, 'H'},
        {"immediate", optional_argument, 0, 'I'},
        {"input", required_argument, 0, 'i'},
        {"job-name", required_argument, 0, 'J'},
        {"kill-on-bad-exit", no_argument, 0, 'K'},
        {"no-kill", no_argument, 0, 'k'},
        {"label", no_argument, 0, 'l'},
        {"licenses", required_argument, 0, 'L'},
        {"clusters", required_argument, 0, 'M'},
        {"distribution", required_argument, 0, 'm'},
        {"nodes", required_argument, 0, 'N'},
        {"ntasks", required_argument, 0, 'n'},
        {"output", required_argument, 0, 'o'},
        {"overcommit", no_argument, 0, 'O'},
        {"partition", required_argument, 0, 'p'},
        {"qos", required_argument, 0, 'q'},
        {"quiet", no_argument, 0, 'Q'},
        {"relative", required_argument, 0, 'r'},
        {"core-spec", required_argument, 0, 'S'},
        {"oversubscribe", no_argument, 0, 's'},
        {"threads", required_argument, 0, 'T'},
        {"time", required_argument, 0, 't'},
        {"unbuffered", no_argument, 0, 'u'},
        {"verbose", no_argument, 0, 'v'},
        {"nodelist", required_argument, 0, 'w'},
        // {"wait", required_argument, 0, 'W'}, // "W:" has special meaning in getopt
                                                // so we implement this below
        {"disable-status", no_argument, 0, 'X'},
        {"exclude", required_argument, 0, 'x'},
        {"no-allocate", no_argument, 0, 'Z'},

        // these don't have short options in srun, but we need to pick a letter
        // for the switch statement. all letters picked here are not used on the
        // srun command line.
        {"ntasks-per-node", required_argument, 0, 'a'},
        {"exclusive", optional_argument, 0, 'f'},
        {"gres", required_argument, 0, 'F'},
        {"wait", required_argument, 0, 'g'},
        {0, 0, 0, 0},
    };

    // given `val`, return the long version of the option
    const auto lookup_long = [&](int val) -> const char* {
        for (const struct option* opt = options; opt->val != 0; opt++) {
            if (opt->val == val) return opt->name;
        }
        return nullptr;
    };

    int opt;
    /* extern int */ optind = 0;

    int ranks = 0;
    int nodes = 0;
    int tasks_per_node = 0;

    // throws on failure
    const auto optstring = optstring_from_longopts(options, OptstringScanningMode::PosixlyCorrect, /* differentiate_missing_arg*/ true);

    while ((opt = getopt_long(argc, argv, optstring.c_str(), options, nullptr)) != -1) {
        switch (opt) {
            case 'n': {
                // in the case of multiple, overwrite old value (matches srun)
                ranks = ::atoi(optarg);
                if (ranks < 1) {
                    throw std::runtime_error(std::string{"Invalid number of tasks: "} + optarg);
                }
                // we don't forward this to launcher args since it will be handled
                // later based on the procset name.
                break;
            };

            case 'N': {
                // FIXME: the full syntax of this argument is --nodes=<minnodes>[-maxnodes]|<size_string>.
                //        if min-max is used, there is no way for use to calculate the number of ranks up front
                //        since srun decides at launch time.
                const auto arg = std::string_view(optarg);
                if (arg.find('-') != std::string_view::npos || arg.find(',') != std::string_view::npos) {
                    throw std::runtime_error(std::string{
                        "gdb4hpc does not support "
                        "--nodes=<minnodes>-<maxnodes>|<size_string> syntax. "
                        "Please use --nodes=<minnodes> or use the launch command "
                        "with --launcher-args."});
                }

                nodes = ::atoi(optarg);
                if (nodes < 1) {
                    throw std::runtime_error(std::string{"Invalid number of nodes: "} + optarg);
                }

                // there is no way to pass the key value argument pair verbatim.
                // since this case handles all of "-N 3", "-N3", "--nodes 3",
                // and "--nodes=3", we don't know if the key/value make up one
                // or two entries in argv. so we always expand to the two item
                // long version
                const auto long_version = std::string{"--"} + lookup_long(opt);
                job_opts.launcher_args << long_version;
                job_opts.launcher_args << optarg;

                break;
            }

            case 'a': {
                tasks_per_node = ::atoi(optarg);
                if (tasks_per_node < 1) {
                    throw std::runtime_error(std::string{"Invalid number of tasks per node: "} + optarg);
                }

                const auto long_version = std::string{"--"} + lookup_long(opt);
                job_opts.launcher_args << long_version;
                job_opts.launcher_args << optarg;

                break;
            }

            // cases where our parser supports the argument, but we don't do anything but forward it
            default: {
                const auto long_version = std::string{"--"} + lookup_long(opt);
                job_opts.launcher_args << long_version;
                if (optarg) job_opts.launcher_args << optarg;

                break;
            }

            // error cases:

            case ':': {
                // valid argument given, but missing argument
                throw std::runtime_error(std::string{"Missing argument after \""} + argv[optind - 1] + "\".");
            }

            case '?': {
                // XXX: we have to quit here because of reasons laid out above and in PE-54906

                const auto erronious = [&](){
                    // the location of the bad option depends on if a long (--long) option was used
                    // or a short (-l) option was used.
                    if (optopt != '\0') return std::string{"-"} + (char)optopt; // short option
                    return std::string{argv[optind - 1]}; // else long
                }();

                // combine the already parsed launcher args into a best effort
                // suggestion for the launch command. the goal here is not to be 100%
                // correct (if we could achieve that, we wouldn't be emitting an error
                // message), but to teach the user how to use the launch command, even
                // if the suggested arguments aren't quite right.
                const auto try_this = [&](){
                    auto result = std::string{"launch "};
                    const auto guess_ranks = ranks ? ranks : 1;
                    result.append("$name{" + std::to_string(guess_ranks) + "} ");
                    result.append("--launcher-args=\'" + job_opts.launcher_args.asString() + " " + erronious + "\' ");
                    for (int i = optind; i < argc; i++) result.append(std::string{argv[i]} + " ");
                    return result;
                }();

                throw std::runtime_error(std::string{"Unsupported srun option: "} + erronious + ", please use the launch command with --launcher-args, best guess: `" + try_this + "`");
            }
        }
    }

    for (; optind < argc; optind++) {
        job_opts.app_argv << argv[optind];
    }
    job_opts.run_argv = job_opts.app_argv;

    if (job_opts.app_argv.argc() == 0) {
        throw std::runtime_error("Not enough arguments. Expected: srun ./program <args>");
    }

    // auto detect python
    // TODO: make this better
    job_opts.python = job_opts.app_argv[0].find("python3") != std::string::npos;

    // now create/update the procset string, which encodes the total ranks.

    const auto additional_ranks = [&](){
        // calculating the number of ranks specified is tricky because of all the (buggy)
        // ways -n/-N/--ntasks-per-node can combine.
        if (ranks) {
            // setting -n overwrites other settings in most cases...
            if (!nodes && tasks_per_node) {
                // ...except this case, which is probably a slurm bug.
                // -n4 --ntasks-per-node=8 results in 8 ranks despite -n4 and what
                // the srun man page says. seen on slurm 23.02.7.
                //
                // -n8 --ntasks-per-node=4 results in 8 properly
                return std::max(ranks, tasks_per_node);
            }

            return ranks;
        } else if (nodes && tasks_per_node) {
            return nodes * tasks_per_node;
        } else if (nodes && !tasks_per_node) {
            return nodes; // one rank per node
        } else if (!nodes && tasks_per_node) {
            return tasks_per_node; // n tasks per node on 1 node
        } else {
            // nothing was given
            return 1;
        }
    }();

    if (opts.procset_name.empty()) {
        auto basename = [&](){
            const auto last_slash = job_opts.app_argv[0].rfind('/');
            if (last_slash != std::string::npos && last_slash != job_opts.app_argv[0].size() - 1) {
                return job_opts.app_argv[0].substr(last_slash + 1);
            } else if (last_slash == std::string::npos) {
                return job_opts.app_argv[0];
            } else {
                // binary name ends in '/'.
                throw std::runtime_error("No such file or directory: " + job_opts.app_argv[0]);
            }
        }();
        // XXX: procset syntax does not allow symbols
        for (auto& c : basename) {
            if (!isalnum(c)) c = '_';
        }
        opts.procset_name = std::string{"$"} + basename + '{' + std::to_string(additional_ranks) + '}';
    } else {
        // XXX: since the number of ranks is encoded in the procset string, and we need
        //      to calculate the correct number of ranks in the case of mpmd, we have to
        //      do weird string-based math here.
        const auto bracket_idx = opts.procset_name.find('{');
        if (bracket_idx == std::string::npos || bracket_idx == opts.procset_name.size() - 1) {
            throw std::runtime_error("Invalid procset name");
        }
        const auto procset_name = opts.procset_name.substr(0, bracket_idx);
        const auto old_ranks = ::atoi(&opts.procset_name[bracket_idx + 1]);
        opts.procset_name = procset_name + '{' + std::to_string(old_ranks + additional_ranks) + '}';
    }

    return job_opts;
}

LaunchOpts LaunchOpts::parse_srun(int argc, const char* const argv[], ClientSettingsInterface& csi) {
    if (argc < 2) {
        throw std::runtime_error("Not enough arguments. Expected: srun ./program <args>");
    }

    // srun [OPTIONS(0)... [executable(0) [args(0)...]]] [ : [OPTIONS(N)...]] executable(N) [args(N)...]

    auto result = LaunchOpts{};

    // it might be tempting to set result.launcher_name to srun here, but that's
    // not necessary - CTI will figure it out.
    //
    // this has another benefit that in theory, a user could paste a basic srun
    // commmand on a PALS machine and the launch will still work, as long as
    // the launcher args weren't too exotic e.g. only using -n.

    for (auto&& argv_section : get_mpmd_sections(argc, argv)) {
        result.job_opts.emplace_back(
            parse_srun_section(result, argv_section.size(), argv_section.data())
        );
    }

    result.read_client_settings(csi);

    return result;
}

void LaunchOpts::read_client_settings(ClientSettingsInterface& csi) {
    this->non_stop_flag = csi.isNonStop();
    this->early_entry_flag = csi.isEarlyEntry();
    this->unsafe_flag = csi.isUnsafe();
    this->proto_name = csi.getProto();
    this->backend_name = csi.getBackend();

    if (csi.getPython() == PythonType::On) {
        for (auto& jopts : job_opts) {
            jopts.python = true;
        }
    } else if (csi.getPython() == PythonType::Off) {
        for (auto& jopts : job_opts) {
            jopts.python = false;
        }
    } else {
        // else auto detected on backend
    }
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

