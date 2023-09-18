//  Copyright 2023 Hewlett Packard Enterprise Development LP.
#pragma once

#include "args.h"

// Bundle up the launch time setting and arguments into a single object 
class LaunchOpts {
 public: // types

  // Most launches will have a single JobOpts that contains the launcher and application arguments
  // However, Slurm MPMD launches can contain multiple JobOpts that apply different launcher
  // arguments to each slice of the MPMD job
  struct JobOpt {

    ArgvStream launcher_args;        // --launcher-args="launcher argv"
                                     // if processType() == DBG_LAUNCHED, stores passed WLM launcher
                                     // arguments. These arguments can't be changed by the user
                                     // after launch.
                                     //
    ArgvStream app_argv;             // --args="application argv"
                                     // if processType() == DBG_LAUNCHED, holds app's argv,
                                     // with the name of the binary as argv[0].
                                     //
    ArgvStream run_argv;             // args to apply when a `run` command is invoked. initially
                                     // holds app_argv, but args can be changed with "set args" 
                                     // command. argv[0] should never change.
                                     //
    bool non_mpi_flag = false;       // --non-mpi
  };

 public:
  LaunchOpts() = default;

  // throws std::runtime_error upon parse error
  LaunchOpts(int argc, const char* const argv[], ClientSettingsInterface& csi);

  // throws std::runtime_error if a held argument is invalid. examples of invalid
  // arguments are a path to a nonexistent binary set as the binary name, a gdb set
  // by --gdb that can't be found in PATH, etc.
  //
  // after calling enforceValid, any stored relative paths will be replaced with full paths.
  void enforceValid();

  std::string launcher_name;         // --launcher="custom_launcher"
  std::vector<JobOpt> job_opts;      // Pack of launcher and application arguments (multiple for MPMD)

  std::string gdb_path = "";         // "--gdb /path/to/custom/gdb" or "--gdb my_gdb" to be found in PATH
  std::string gdb_data_dir;          // --gdb-data-dir
  std::string input_file_path = "";  // --launcher-input /path/to/input/file
  std::string cwd = "";              // --workdir /path/to/workdir
  bool set_default_cwd = false;      // true if --workdir is passed with no argument. if true,
                                     // the cwd of the program should be set to the directory
                                     // the actual binary file is located in.
  ArgvStream env;                    // --env "name=value"
  bool gpu_flag = false;             // --gpu
  bool debug_flag = false;           // --debug
  std::string qsub_file_path = "";   // --qsub /path/to/script
  std::string sbatch_file_path = ""; // --sbatch /path/to/script
  std::string procset_name;          // full name of passed procset, including rank count, e.g. $a{3}
                                     //
  bool non_stop_flag = true;         // settings copied from passed ClientSettingsInterface
  bool early_entry_flag = false;     //
  bool unsafe_flag = false;          //
  std::string proto_name = "";       // proto is a legacy setting that doesn't change
  std::string backend_name = "";     //
};

// Connection options for attaching to a running process
struct AttachOpts {
  std::string proto;
  std::string app_ident;
  std::string backend;
  std::string gdb;
  std::string gdb_data_dir;
  std::string vgdb_file;
  bool gpu_flag = false;
  bool debug_flag = false;
};


