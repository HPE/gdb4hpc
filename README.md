# gdb4hpc

gdb4hpc is a GDB-based parallel debugger for debugging applications running
across many processes and compute nodes. It provides a command-line interface
modeled on GDB, while adding HPC-oriented features such as process sets,
parallel command execution, response aggregation, and comparative debugging.

This README is the project entry point. It gives a brief overview, points to
the appropriate documentation, and provides a temporary outline of the build
instructions. The build section should be expanded with complete prerequisites,
configuration, installation, and platform-specific instructions.

## Documentation

### New users

Start with the [Getting Started Guide](doc/guides/getting-started.md). It
introduces the help system, launching an MPI application, debugging basics,
procsets, and focusing on a subset of application ranks.

### User documentation

The [user documentation index](doc/cpedocs/index.md) links to the complete
gdb4hpc guides, tutorials, man pages, and command reference. It includes
information about HPC features, CUDA debugging, array handling, parallel
programming libraries, the VS Code extension, and Python debugging.

### Developers

Developers should begin with the [internal structural overview](doc/internal/overview.md),
which describes the client, debug-client, communication, and server layers and
how they work together.

## Features

This section should summarize the supported workflows and major capabilities,
including:

- Debugging distributed and parallel applications.
- Executing debugger commands over selected process sets.
- Aggregating results from many ranks into manageable responses.
- Comparative debugging of multiple application runs.
- Support for common HPC launchers and programming environments.
- Debugging support for arrays, Python applications, and GPU applications.

## Requirements

Gdb4hpc depends to two other HPE open source projects:

- cti-devel
- zmqnet

And dependencies generally available from the OS:

- meson
- bison
- libdw-devel
- libuuid-devel
- ncurses
- libbz2-devel
- python3


## Build and Install

### Dual build modes

This version of gdb4hpc supports building either stand-alone or within the HPE build pipelines.   This is mosly confinded to the 'hpe' directory, but there are some ".sh" and ".yaml" files that are required to live in the top level directory.   Also, the HPE build relies on submodules.  For the open source build, _you do not need to `git submodule update`_ which would fail because they refer to internal repositories.    

### Building HPE tools in a shared directory

Given a shared prefix `$prefix`, you can build cti, zmqnet and gdb4hpc in turn with:

```bash
export PKG_CONFIG_PATH=$prefix/lib/cti/pkgconfig:$prefix/lib/zmqnet/pkgconfg:$PKG_CONFIG_PATH

for i in cti zmqnet gdb4hpc; do
   pushd $i
   meson setup -Dprefix=$prefix -Dlibdir=lib/$i -Dlibexecdir=libexec/$i ./build
   popd
done
```

Assuming you've installed the prequisites for all three packages.

One could potentially create cti.wrap and zmqnet.wrap files to build the dependencies as meson subprojects, but the build definitions are not expected to be stable enough yet for that approach to be reliable.

### Building HPE rpms.

The usual build scripts runBuildPrep.sh, runBuild.sh and runBuildPackage.sh are defined in the hpe directory, which run in the usual sequence.  That build will depend on the cray-cti-devel, and zmqnet rpms and their associated module files.

### Change configuration to a debug build

```
meson setup --reconfigure -Ddebug=true -Doptimization=0 ./build
meson compile -C build
```

## Testing

This section should document the test prerequisites, available test suites,
standard test commands, and how to interpret or troubleshoot failures.

### Run unit tests

```
meson test [--verbose] -C build
```

## Packaging and Release

Building rpms is only set up for the HPE generated release.   The adaptation 
required to create a stand-alone one is would not be too great, but was 
beyond the scope of an intial release.   hpe/rpmbuild/SPECS/gdb4hpc.spec 
would be a good starting point; it has all the right files, but most are in 
different directories.

### Build a package

```
BUILD_METADATA=0 ./hpe/runBuildPackage.sh
```

## Development

See the [internal developer documentation](doc/internal/overview.md). This
section should also describe the development workflow, source tree layout,
coding conventions, contribution process, and debugging techniques.

## Contributing

Feel free to submit a PR at any time.   Any code submitted will be subject to the 
same licensing terms as the rest of gdb4hpc.

## License and Attribution

See ATTRIBUTIONS_gdb4hpc.txt


