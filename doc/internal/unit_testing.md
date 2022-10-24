# Unit Tests in `gdb4hpc`

Josh Chandler

10/21/2022

---

`gdb4hpc` uses the `doctest` C++ unit testing framework. `doctest` tries to
emulate the testing style of languages like Rust, D, and Nim, where unit tests
are written as part of the source code itself.

This means that to write unit tests, you don't have to edit and build a separate
binary, and in order for `gdb4hpc` to build, the unit tests have to build too.

## Running Tests

To run `gdb4hpc` unit tests, run `gdb4hpc` with the `--test` option:

```bash
$ gdb4hpc --test
[doctest] doctest version is "2.4.9"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:   19 |   19 passed | 0 failed | 0 skipped
[doctest] assertions: 3517 | 3517 passed | 0 failed |
[doctest] Status: SUCCESS!
```

For test running options, such as running only a subset of tests, use options
from `gdb4hpc --test --help`:

```bash
$ gdb4hpc --test --help

...

 -s,   --success=<bool>                include successful assertions in output
... 
 -tc,  --test-case=<filters>           filters tests by their name

...

```

```bash
$ gdb4hpc --test -tc="DbgInvoke*" -s
[doctest] doctest version is "2.4.9"
[doctest] run with "--help" for options
===============================================================================
dbginvoke.cpp:16:
TEST CASE:  DbgInvoke: operator==
  operator==

dbginvoke.cpp:30: SUCCESS: REQUIRE( dip != nullptr ) is correct!
  values: REQUIRE( 0x8a1b90 != nullptr )

dbginvoke.cpp:31: SUCCESS: REQUIRE( dip2 != nullptr ) is correct!
  values: REQUIRE( 0x8a1b90 != nullptr )

dbginvoke.cpp:33: SUCCESS: CHECK( *dip == *dip2 ) is correct!
  values: CHECK( {?} == {?} )

...

```

`dbgsrv` also has unit tests:

```bash
$ $(dirname $(which gdb4hpc))/../libexec/dbgsrv --test
Registered server protocol: mrnet
Registered server protocol: zmqnet
[doctest] doctest version is "2.4.9"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:   15 |   15 passed | 0 failed | 0 skipped
[doctest] assertions: 3495 | 3495 passed | 0 failed |
[doctest] Status: SUCCESS!
```

## Writing Tests

### Test Cases

Tests are written in `TEST_CASE` macros:

```cpp
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

    // clean up as to not affect other tests (see note)
    DbgInvoke::deactivateAll();
}
```

`TEST_CASE`s can contain `SUBCASE`s, which is `doctest`'s replacement for the
traditional `setup()` and `teardown()` fixture pattern.

The effects of each `SUBCASE` are invisible to the other `SUBCASE`s:

```cpp
TEST_CASE("vectors can be sized and resized") {
    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SUBCASE("adding to the vector increases it's size") {
        v.push_back(1);

        CHECK(v.size() == 6);
        CHECK(v.capacity() >= 6);
    }
    SUBCASE("reserving increases just the capacity") {
        v.reserve(6);

        CHECK(v.size() == 5);
        CHECK(v.capacity() >= 6);
    }
}
```

The `REQUIRE` macro will stop the rest of the test on failure, while the `CHECK`
macro will mark a test as failed, but won't stop the rest. `CHECK` is good for
assertions that don't depend on each other, while `REQUIRE` is good for setting
up initial conditions.

Note: even when using `SUBCASE`s, modification of static global data (like the
affects of `DbgInvoke::activate` above) *can* be observed in independent tests.

See https://github.com/doctest/doctest/tree/master/doc/markdown for more info.

### Test Case Headers

In `gdb4hpc` there are specific unit test headers defined. Most of the time, you
will just need `unittest_macros.h`.

```
// ready to compile, link, and test
#include "unittest_macros.h"

TEST_CASE("...") {
    ...
}
```

`unittest_macros.h` lives in `/src/unittest`. Not all subdirectories of the
`gdb4hpc` know where this is yet. If you get an include error, update the
`Makefile.am` in the directory you're trying to build in. See
`/src/useful/Makefile.am` for an example of how to include the header directory.

### Test Driver Headers

To run unit tests, the `main` function of `gdb4hpc` (and `dbgsrv`) needs to
include `unittest_drivers.h` and call `runUnitTests()`.

```
...

#include "unittest_drivers.h"

...

int main(int argc, char *argv[]) {

...

    if (argc >= 2 && strcmp(argv[1], "--has-tests") == 0) {
        return hasUnitTests();
    }

    if (argc >= 2 && strcmp(argv[1], "--test") == 0) {
        argv[1] = argv[0];
        return runUnitTests(argc - 1, &argv[1]);
    }

...

```

## Build Details

This section is about how `gdb4hpc` is instrumented with `doctest` at configure,
build, and link time.

### `doctest` Implementation

Since C++ doesn't support test cases like this on a language level, `doctest`
has to do some tricks to make it feel like it does.

In particular, `doctest` has to do two things:

1. Register its tests somehow
2. Hook into `main` somehow to run them

Number 2 is covered in the `unittest_drivers.h` header, but number 1 is
automatic. To automatically register tests, code with `doctest` tests *needs* to
be able to find a `doctest` "**implementation**", which the tests talk with to
register themselves and the runner talks with to run tests.

### `doctest` Implentation in a Shared Library

The need for an implementation has consequences at link time. If a piece of code
with tests is linked with other code, and that other code doesn't have `doctest`
implementation symbols, linking errors will occur.

This applies to shared library loads at runtime via `dlopen`. In particular, the
`gdb4hpc` aggregation filters are shared libraries. However, the aggregation
filters are not loaded by `gdb4hpc`, but by `zmqnet`'s `commnode`s. `commnode`s
*don't* have a `doctest` implementation (yet), but the filters have tests that
need one (they use bitsets, which have tests).

If left alone in this state, the `commnode` will crash when trying to `dlopen`
the aggregation filter, because static test registration code that runs at link
time will look for implementation symbols and not find them.

The solution is to build the `doctest` implementation as a shared library itself,
and then have the bitset library link to it as a shared library dependency at
build time. This cascades down to the aggregation filter, which depends on the
bitset library, which depends on the `doctest` implementation library. The end
result is that the bitset library knows where to look for its needed
implementation symbols at `dlopen` time as everything works.

This implementation library is built as `libgdb4_unittest.so` and shipped with
`gdb4hpc`.

### Disabling Tests for Release

`doctest` has a configuration setting that completely disables tests. We do this
for release builds.

To disable tests, globaly define `DOCTEST_CONFIG_DISABLE`. Here, "globaly define"
means that every translation unit should have the definition visible.

### Configure Time Options

`gdb4hpc`'s `configure.ac` is set up to accept two `doctest` related options

1. `--with-doctest=<path>`

This option is how Makefiles know where `doctest` is. Even when disabling tests,
it *must* be provided. The path should point to the root of the `doctest`
submodule.

2. `--disable-doctest`

This option globally defines `DOCTEST_CONFIG_DISABLE`, removing all test code
from the resulting binary.

Note that it does this by defining the macro in `config.h`. This means that any
file with tests should also include `config.h` in order to respect the toggle.
`unittest_*.h` handle this.
