# gdb4hpc

## Build From Scratch, Including Dependencies

This build must be run at least once before the rest of the build types can be run.
This configures gdb4hpc to be built in release mode (optimizations on).
See below for debug mode.

```
./runBuildPrep.sh
./runBuild.sh
```

## Build Only gdb4hpc

```
meson compile [--verbose] -C build
```

Alternatively:

```
ninja [--verbose] -C build
```

## Build Only A Single Object File

`meson compile` will be adequate 99% of the time, but you can build a single object via ninja:

```
ninja -C build gdb4hpc.p/src_client_cmd_frame.cpp.o
```

Check the `build.ninja` for target names or use `ninja -C build -t targets all`.

## Change Configuration to a Debug Build

```
meson setup --reconfigure -Ddebug=true -Doptimization=0 ./build
meson compile -C build
```

## Run Unit Tests

```
meson test [--verbose] -C build
```

## Build Package

```
BUILD_METADATA=0 ./runBuildPackage.sh
```

## For develoment and debugging information

See files in doc/internal


