project(
  'gdb4hpc',
  ['c', 'cpp'],
  version: run_command(
    'bash',
    '-c', 'source '
    + meson.current_source_dir()
    + '/release_versioning && echo "$major.$minor.$revision.$build_number"',
    check: true,
  ).stdout().strip(),
  default_options: [
    'cpp_std=c++20',
    'warning_level=2', # equivalent to -Wall -Wextra
    'werror=true',
  ],
)

# for use in generated documentation. since cpedocs requires that we commit documentation to
# gdb4hpc's repo for easy fetching, we never want the master branch to contain docs that say
# e.g. "for gdb4hpc 4.16.3.9999".
version_short = run_command(
  'bash',
  '-c', 'source '
  + meson.current_source_dir()
  + '/release_versioning && echo "$major.$minor.$revision"',
  check: true,
).stdout().strip()

# external dependencies ----------------------------------------------------------------

cti_fe_dep = dependency(
  'common_tools_fe',
  method: 'pkg-config',
  not_found_message: 'Try `module load cray-cti-devel` first.',
)

cti_be_dep = dependency(
  'common_tools_be',
  method: 'pkg-config',
  not_found_message: 'Try `module load cray-cti-devel` first.',
)

zmqnet_dep = dependency(
  'libzmqnet',
  method: 'pkg-config',
  not_found_message: 'Try `module load cray-zmqnet` first.',
)

mrnet_dep = dependency(
  'cray-mrnet',
  method: 'pkg-config',
  not_found_message: 'Try `module load cray-mrnet` first.',
)

# for boost (used in mrnet proto)
cdst_support_dep = dependency(
  'cray-cdst-support',
  method: 'pkg-config',
  not_found_message: 'Try `module load cray-cdst-support` first.',
)

py_module = import('python')

# the system python installation is used for the python interpreter embedded in
# the client. for pybe, we are more explicit about the versions we use (see libgdb4_pybe below).
system_python_installation = py_module.find_installation('python3')
system_python_dep = system_python_installation.dependency(embed: true)
python_compat_dep = declare_dependency(
  include_directories: ['./external/pythoncapi-compat'],
)

# from cdst support
libedit_dep = dependency('libedit', method: 'pkg-config')

# from cdst support
libdw_dep = dependency('libdw', method: 'pkg-config')

# from zmqnet
libLrleBitset_dep = dependency('libLrleBitset', method: 'pkg-config')

doctest_proj = subproject(
  'doctest',
)
doctest_dep = doctest_proj.get_variable('doctest_dep')

pthread_dep = dependency('threads') # 'threads' is a magic meson builtin

# from zmqnet
capnp_prog = find_program(
  [
    'capnp', # in PATH
    zmqnet_dep.get_variable(pkgconfig: 'prefix') + '/support/bin/capnp',
  ],
)

bison_prog = find_program('bison')

python3_prog = find_program('python3')

# config -------------------------------------------------------------------------------

conf = configuration_data()

conf.set_quoted('GDB4HPC_VERSION', meson.project_version(), description: 'the gdb4hpc version including build number, e.g. 4.16.3.99')
conf.set_quoted('GDB4HPC_VERSION_SHORT', version_short, description: 'the gdb4hpc version not including build number, e.g. 4.16.3')

conf.set('ZMQNET', 1, description: 'legacy. always 1.')
conf.set('MRNET', 1, description: 'legacy. always 1.')
conf.set('ALPS', 1, description: 'legacy. always 1.')

# internal dependencies ----------------------------------------------------------------
#
# these are part of the gdb4hpc build/repo, but are designed/used as if they
# are standalone libraries.

libgdb4_unittest_so = shared_library(
  'gdb4_unittest',
  sources: [
    './src/unittest/unittest_impl.cpp',
  ],
  include_directories: [
    './src/unittest',
  ],
  dependencies: [
    doctest_dep,
  ],
  install: true,
  install_dir: './lib',
)

# prefer using this dependency over hard coding the path to ./external/doctest
unittest_dep = declare_dependency(
  include_directories: [
    './src/unittest',
  ],
  dependencies: [
    doctest_dep,
  ],
  link_with: [
    libgdb4_unittest_so,
  ],
)

libmi_proj = subproject(
  'libmi',
  default_options: [
    'werror=false', # libmi is known to have a few warnings
  ],
)
libmi_dep = libmi_proj.get_variable('libmi_dep')

# prefer using this dependency over hard coding the path to ./external/tinystl
tinystl_dep = declare_dependency(
  include_directories: [
    './external/tinystl/include',
  ],
)

# binaries ----------------------------------------------------------------------------

libgdb4_bitset_a = static_library(
  'gdb4_bitset',
  sources: [
    './src/bitset/bitset.cpp',
    './src/bitset/dense_bitset.cpp',
    './src/bitset/sparse_bitset.cpp',
    './src/bitset/lrunle_bitset.cpp',
    './src/bitset/bitset_convert.cpp',
  ],
  include_directories: [
    './src',
  ],
  dependencies: [libLrleBitset_dep, unittest_dep],
)

libgdb4_useful_a = static_library(
  'gdb4_useful',
  sources: [
    './src/useful/alloc.cpp',
    './src/useful/args.cpp',
    './src/useful/debug.cpp',
    './src/useful/error.cpp',
    './src/useful/exec.cpp',
    './src/useful/hashfunctions.cpp',
    './src/useful/log.cpp',
    './src/useful/maths.cpp',
    './src/useful/path.cpp',
    './src/useful/queue.cpp',
    './src/useful/str.cpp',
    './src/useful/onscopeexit.cpp',
    './src/useful/parse.cpp',
    './src/useful/signal_util.cpp',
  ],
  include_directories: [
    './src',
  ],
  dependencies: [unittest_dep],
)

dbg_zmq_capnp = custom_target(
  input: ['./src/dbgcomm/dbg_zmq.capnp'],
  output: [
    'dbg_zmq.capnp.h',
    'dbg_zmq.capnp.c++',
  ],
  env: {
    # the capnp binary expects its individual language plugins to be in PATH
    'PATH': zmqnet_dep.get_variable(pkgconfig: 'prefix') + '/support/bin',
  },
  command: [
    capnp_prog,
    'compile',
    '-oc++',
    '--src-prefix', meson.current_source_dir() + '/src/dbgcomm',
    '@INPUT@',
  ],
)

libgdb4_dbg_a = static_library(
  'gdb4_dbg',
  sources: [
    './src/dbgcomm/dbgbp.cpp',
    './src/dbgcomm/dbgevent.cpp',
    './src/dbgcomm/dbgformat.cpp',
    './src/dbgcomm/AIFStream.cpp',
    './src/dbgcomm/ais.cpp',
    './src/dbgcomm/blockmap.cpp',
    './src/dbgcomm/socstream.cpp',
    dbg_zmq_capnp,
  ],
  include_directories: [
    './src',
    './src/bitset',
    './src/useful',
  ],
  dependencies: [
    zmqnet_dep,
    unittest_dep,
  ],
)

libgdb4_dmr_a = static_library(
  'gdb4_dmr',
  sources: [
    './src/filters/dmr.cpp',
  ],
  include_directories: [
    './src/filters',
    './src/bitset',
  ],
)

libgdb4_dbgagg_so = shared_library(
  'gdb4_dbgagg',
  sources: [
    './src/filters/mrnet/Aggregation.cpp',
  ],
  include_directories: [
    './src/filters',
    './src/bitset',
  ],
  dependencies: [
    mrnet_dep,
    cdst_support_dep,
    unittest_dep,
  ],
  link_with: [
    libgdb4_dmr_a,
    libgdb4_bitset_a,
  ],
  install: true,
  install_dir: './lib',
)

libgdb4_zmqnetagg_so = shared_library(
  'gdb4_zmqnetagg',
  sources: [
    './src/filters/zmqnet/zmqnetagg.cpp',
    dbg_zmq_capnp,
  ],
  include_directories: [
    './src/bitset',
  ],
  dependencies: [
    zmqnet_dep,
    libLrleBitset_dep,
    unittest_dep,
  ],
  link_with: [
    libgdb4_bitset_a,
  ],
  install: true,
  install_dir: './lib',
)

expr_tab = custom_target(
  input: ['./src/gram/expr.y'],
  output: ['expr.tab.hpp', 'expr.tab.cpp'],
  command: [
    bison_prog,
    '-Wno-yacc',
    '-b', 'expr',
    '-p', 'expr_',
    '-d',
    '-v',
    '-o', 'expr.tab.cpp',
    '@INPUT@',
  ],
)

libgdb4_gram_a = static_library(
  'gdb4_gram',
  sources: [
    './src/gram/gram.cpp',
    './src/gram/val.cpp',
    './src/gram/eval.cpp',
    './src/gram/blockmap_client.cpp',
    './src/gram/procset.cpp',
    './src/gram/slice.cpp',
    './src/gram/var.cpp',
    expr_tab,
  ],
  include_directories: [
    './src',
    './src/client',
    './src/gram',
    './src/dbgclnt',
    './src/dbgcomm',
    './src/useful',
    './src/bitset',
  ],
  dependencies: [
    unittest_dep,
  ],
)

libgdb4_dbgclnt_a = static_library(
  'gdb4_dbgclnt',
  sources: [
    './src/dbgclnt/dbg_client.cpp',
    './src/dbgclnt/dbgclnt_gdbmi.cpp',
    './src/dbgclnt/dbgclnt_proto.cpp',
    './src/dbgclnt/dbgclnt_proto_ctiship.cpp',
    './src/dbgclnt/dbgclnt_network.cpp',
    './src/dbgclnt/proto_message.cpp',
    './src/dbgclnt/dbgclnt_breakpoints.cpp',
    './src/dbgclnt/dbginvoke.cpp',
    './src/dbgclnt/dbgoutp.cpp',
    './src/dbgclnt/observable.cpp',
    './src/dbgclnt/dwarfinfo.cpp',
    './src/dbgclnt/dbgclnt_mrn.cpp',
    './src/dbgclnt/dbg_mrn_network.cpp',
    './src/dbgclnt/mrn_topgen.cpp',
    './src/dbgclnt/dbgclnt_zmq.cpp',
    './src/dbgclnt/dbg_zmq_network.cpp',
    './src/dbgclnt/launchopts.cpp',
  ],
  include_directories: [
    './src',
    './src/dbgclnt',
    './src/dbgcomm',
    './src/useful',
    './src/script',
    './src/gram',
    './src/bitset',
  ],
  dependencies: [
    cti_fe_dep,
    cdst_support_dep,
    mrnet_dep,
    zmqnet_dep,
    libmi_dep,
    unittest_dep,
  ],
)

dbgsrv_alps = executable(
  'dbgsrv_alps',
  ['./src/dbgsrv/dbgsrv_alps_mrn.cpp'],
  include_directories: [
    './src',
    './src/useful',
  ],
  dependencies: [
    cti_be_dep,
    unittest_dep,
  ],
  link_with: [
    libgdb4_useful_a,
  ],
  install: true,
  install_dir: './libexec',
)

test('dbgsrv_alps unit tests', dbgsrv_alps, args: ['--test'])

libgdb4_unversioned_basename = 'gdb4_pybe_'
conf.set_quoted('PYBE_SO_UNVERSIONED_BASENAME', 'lib' + libgdb4_unversioned_basename,
  description: 'Unversioned name of the pybe shared library that will be injected into the python interpreter when debugging python 3.'
)

libgdb4_pybe_sources = [
  './src/dbgsrv/pybe_entry.cpp',
  './src/dbgsrv/pybe_impl.cpp',
  './src/dbgsrv/pybe_stl.cpp',
  './src/dbgsrv/pybe_comms_pybe.cpp',
]
libgdb4_pybe_cpp_args = [
  '-fno-rtti',
  '-fno-exceptions',
]
libgdb4_pybe_link_args = [
  '-lsupc++', # this is enough to tell g++ to exclude libstdc++
  '-Wl,--exclude-libs,ALL', # don't expose symbols in any .a files we link to build this .so
]
libgdb4_pybe_deps = [
  python_compat_dep,
  pthread_dep,
  tinystl_dep,
]

supported_python_versions = ['3.6', '3.8', '3.9', '3.10', '3.11', '3.12']
conf.set('SUPPORTED_PYTHON_VERSIONS', '{"' + '", "'.join(supported_python_versions) + '"}', description: 'Initializer list of supported python versions')
conf.set_quoted('SUPPORTED_PYTHON_VERSIONS_STR', ', '.join(supported_python_versions), description: 'String of supported python versions')

foreach py_vers : supported_python_versions
  libgdb4_pybe_so = shared_library(
    libgdb4_unversioned_basename + py_vers,
    sources: libgdb4_pybe_sources,
    include_directories: [
      './external/python-headers/include/python' + py_vers,
      './src/useful'
    ],
    override_options: [
      # allow undefined symbols. this gets us a very fast build since we don't
      # need to rely on getting n different versions of python installed to
      # link with (which would require building n different versions of python
      # from source). we can count on the fact that any symbol exposed in
      # Python.h will be present in libpython at runtime.
      #
      # however, this option does create a blind spot for our *own* undefined
      # symbols, but see libgdb4_pybe_insurance_so below.
      'b_lundef=false',
    ],
    cpp_args: libgdb4_pybe_cpp_args,
    dependencies: libgdb4_pybe_deps,
    link_args: libgdb4_pybe_link_args,
    link_with: [],
    gnu_symbol_visibility: 'hidden',
    install: true,
    install_dir: './lib',
  )

  pybe_unittest = executable(
    'pybe_unittest_' + py_vers,
    ['./src/dbgsrv/pybe_unittest.cpp'],
    include_directories: [
      './external/python-headers/include/python' + py_vers,
    ],
    dependencies: [
      unittest_dep,
      libgdb4_pybe_deps,
    ],
    link_with: [],
    install: false,
  )

  test('pybe unit tests ' + py_vers, pybe_unittest)
endforeach

# build *and link* a version of libgdb4_pybe against the system python as
# insurance against the undefined symbol blind spot in the normal
# libgdb4_pybe_so build. this library is never seen or used by the user, but
# will flag build issues early for developers.
libgdb4_pybe_insurance_so = shared_library(
  'gdb4_pybe_insurance',
  sources: libgdb4_pybe_sources,
  include_directories: [
    './src/useful'
  ],
  cpp_args: libgdb4_pybe_cpp_args,
  dependencies: [libgdb4_pybe_deps, system_python_dep],
  link_with: [],
  link_args: libgdb4_pybe_link_args,
  install: false,
)

dbgsrv = executable(
  'dbgsrv',
  sources: [
    './src/dbgsrv/dbgsrv.cpp',
    './src/dbgsrv/dbgfn.cpp',
    './src/dbgsrv/dbgsrv_proto.cpp',
    './src/dbgsrv/dbgsrv_cmds.cpp',
    './src/dbgsrv/dbgsrv_gdbmi.cpp',
    './src/dbgsrv/dbgsrv_init.cpp',
    './src/dbgsrv/dbgsrv_libmi2aif.cpp',
    './src/dbgsrv/dbgsrv_typeparser.cpp',
    './src/dbgsrv/dbgsrv_util.cpp',
    './src/dbgsrv/dbgsrv_p2p.cpp',
    './src/dbgsrv/dbg_mrn_be.cpp',
    './src/dbgsrv/dbgsrv_mrn.cpp',
    './src/dbgsrv/mrn_topo.cpp',
    './src/dbgsrv/dbgsrv_zmq.cpp',
    './src/dbgsrv/dbgsrv_pybe.cpp',
    './src/dbgsrv/dbgsrv_pybe_rpc.cpp',
    './src/dbgsrv/pybe_comms_dbgsrv.cpp',
    './src/dbgsrv/mi_utils.cpp',
  ],
  include_directories: [
    './src',
    './src/dbgcomm',
    './src/useful',
    './src/bitset',
  ],
  dependencies: [
    cti_be_dep,
    mrnet_dep,
    zmqnet_dep,
    cdst_support_dep,
    libmi_dep,
    unittest_dep,
    tinystl_dep,
  ],
  link_with: [
    libgdb4_dbg_a,
    libgdb4_useful_a,
    libgdb4_bitset_a,
  ],
  install: true,
  install_dir: './libexec',
)

test('server unit tests', dbgsrv, args: ['--test'])

config_h = configure_file(
  output: 'config.h',
  configuration: conf,
)

help_web = custom_target(
  input: [
    './doc/help/help_input.md',
    './doc/help/generate_help_web.py',
  ],
  output: ['help.md', 'help-web.md'],
  command: [
    python3_prog,
    meson.current_source_dir() + '/doc/help/generate_help_web.py',
    meson.current_source_dir() + '/doc/help/help_input.md',
    config_h,
    '@OUTPUT@',
  ],

  install: true,
  # note that each directory entry maps to each output,
  # so help.md (output[0]) goes to /opt, and help-web goes to ./doc
  install_dir: [
    './share/doc',
    meson.current_source_dir() + '/doc/help', # for cpedocs, see /doc/help/README.md
  ]
)

# TODO: hook the rest of the .md files up here and use pandoc to generate html for all of them.

help_sources = custom_target(
  input: [
    './doc/help/help_input.md',
    './doc/help/generate_help_source.py',
  ],
  output: ['help_dict.h', 'help_dict.cpp'],
  command: [
    python3_prog,
    meson.current_source_dir() + '/doc/help/generate_help_source.py',
    meson.current_source_dir() + '/doc/help/help_input.md',
    config_h,
    '@OUTPUT@',
  ],
)

libgdb4_df_a = static_library(
  'gdb4_df',
  sources: [
    './src/script/script_client.cpp',
    './src/script/script_data.cpp',
    './src/script/script_settings.cpp',
    help_sources[0],
  ],
  include_directories: [
    './src',
    './src/dbgcomm',
    './src/gram',
    './src/dbgclnt',
    './src/useful',
    './src/client',
    './src/bitset',
  ],
)

gdb4hpc = executable(
  'gdb4hpc',
  sources: [
    './src/client/main.cpp',
    './src/client/cli_funcs.cpp',
    './src/client/cmd.cpp',
    './src/client/command_invoke.cpp',
    './src/client/command_invoke_tests.cpp',
    './src/client/cmd_assign.cpp',
    './src/client/cmd_break.cpp',
    './src/client/cmd_checkpoint.cpp',
    './src/client/cmd_comp.cpp',
    './src/client/cmd_env.cpp',
    './src/client/cmd_iostat.cpp',
    './src/client/cmd_frame.cpp',
    './src/client/cmd_go.cpp',
    './src/client/cmd_list.cpp',
    './src/client/cmd_print.cpp',
    './src/client/cmd_proc.cpp',
    './src/client/cmd_procset.cpp',
    './src/client/cmd_source.cpp',
    './src/client/cmd_step.cpp',
    './src/client/cmd_thread.cpp',
    './src/client/cmd_whatis.cpp',
    './src/client/cli_readline.cpp',
    './src/client/cli_socket.cpp',
    './src/client/cli_file.cpp',
    './src/client/cmd_passthru.cpp',
    './src/client/cmd_script.cpp',
    './src/client/cmd_blockmap.cpp',
    './src/client/cmd_utils.cpp',
    './src/client/mi_funcs.cpp',
    './src/client/cmd_python.cpp',
    './src/client/cmd_interpreter.cpp',
    './src/client/interpreter.cpp',
    './src/client/interpreter_console.cpp',
    './src/client/interpreter_mi.cpp',
    './src/client/bttree.cpp',
    help_sources,
  ],
  include_directories: [
    './src',
    './src/script',
    './src/dbgclnt',
    './src/dbgcomm',
    './src/useful',
    './src/gram',
    './src/bitset',
  ],
  dependencies: [
    zmqnet_dep,
    mrnet_dep,
    cti_fe_dep,
    cdst_support_dep,
    system_python_dep,
    libedit_dep,
    libdw_dep,
    libmi_dep,
    unittest_dep,
  ],
  link_with: [
    libgdb4_df_a,
    libgdb4_dbgclnt_a,
    libgdb4_gram_a,
    libgdb4_useful_a,
    libgdb4_dbg_a,
    libgdb4_bitset_a,
  ],
  install: true,
  install_rpath: '$ORIGIN/../lib',
)

test('client unit tests', gdb4hpc, args: ['--test'])
