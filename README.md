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

### doctest and Spack

Meson first looks for an installed doctest using its CMake package configuration.
If none is found, standalone builds download the pinned version in
`subprojects/doctest.wrap`. CMake is needed to discover an installed doctest;
the wrap fallback does not require it.

If doctest was already downloaded before updating these build files, refresh its
include-path overlay with `meson subprojects update --reset doctest`.

The Spack recipe should declare both dependencies:

```python
depends_on("cmake", type="build")
depends_on("doctest", type="build")
```

doctest is header-only, so no doctest runtime library is needed. Spack supplies
dependency search paths and disables automatic wrap downloads; keep that policy
enabled. For a standalone build using doctest in a custom prefix, pass
`-Dcmake_prefix_path=/path/to/doctest` to `meson setup`.

When building the pinned `gdb4hpc@4.17.0` commit, the recipe must also apply these
Meson, header-include, and wrap changes as a patch, or select a commit containing
them. Editing a separate source checkout does not change a normal Spack build
of a pinned commit.

### Other wrapped dependencies in Spack

tinystl and pythoncapi-compat can be staged as Spack resources instead of
downloaded by Meson. This keeps standalone wrap builds unchanged and lets Spack
fetch or mirror their sources before the build. Add these resources to the
gdb4hpc recipe, using the same commits as the corresponding wrap files:

```python
resource(
    name="tinystl",
    git="https://github.com/mendsley/tinystl.git",
    commit="77bfd2f32c373180ac01798a1faa95142d65288b",
    destination="subprojects",
    placement="tinystl",
)
resource(
    name="pythoncapi-compat",
    git="https://github.com/python/pythoncapi-compat.git",
    commit="0041177c4f348c8952b4c8980b2c90856e61c7c7",
    destination="subprojects",
    placement="pythoncapi-compat",
)
```

Meson does not apply wrap overlays or patches to directories already staged by
Spack. Add `from spack.patch import apply_patch` to the recipe's imports, then
prepare those directories in the recipe's patch phase:

```python
def patch(self):
    source = self.stage.source_path
    overlays = join_path(source, "subprojects", "packagefiles")
    for name in ("tinystl", "pythoncapi-compat"):
        install(
            join_path(overlays, name, "meson.build"),
            join_path(source, "subprojects", name, "meson.build"),
        )
    apply_patch(
        source,
        join_path(overlays, "tinystl", "hpe.patch"),
        working_dir=join_path("subprojects", "tinystl"),
    )
```

The tinystl patch is required: gdb4hpc uses API additions that are not in the
upstream headers. Do not substitute an unpatched tinystl installation. Keep
resource commits synchronized with the wraps when updating dependencies.

Some Spack versions skip resource staging when an existing checkout is selected
through `develop`. For those versions, the recipe also needs to stage its
resources explicitly in that case. Add `import os`, `import spack.config`, and
`from spack.stage import ResourceStage` to the recipe, then:

```python
def do_stage(self, mirror_only=False):
    super().do_stage(mirror_only=mirror_only)
    dev_path = spack.config.get(f"develop:{self.name}:path")
    if dev_path and os.path.exists(dev_path):
        for stage in self.stage:
            if isinstance(stage, ResourceStage):
                stage.fetch(mirror_only=mirror_only)
                if spack.config.get("config:checksum"):
                    stage.check()
                stage.cache_local()
                stage.expand_archive()
```

This uses Spack's resource fetchers and mirror policy without fetching or
replacing the development checkout itself. Do not use `--skip-patch`: the patch
phase installs the resource overlays and applies the tinystl changes.

### Cap'n Proto compiler and C++ plugin

gdb4hpc runs `capnp compile` during the build and needs both `capnp` and
`capnpc-c++`. Meson looks for the C++ plugin beside the selected compiler first,
then on `PATH`, and passes its path explicitly rather than replacing `PATH`
with zmqnet's installation directories. Standalone builds can still use the
tools bundled under zmqnet's `bin` or `support/bin`.

The Spack recipe should declare Cap'n Proto directly, matching zmqnet's patched
version so generated code and library headers agree:

```python
depends_on("capnproto@=0.8-dev", type=("build", "link"))
```

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
