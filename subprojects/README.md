# Meson Subprojects

This directory is *required* by meson for subprojects. meson will not accept
any other directory.

To keep our existing convention of keeping subprojects in `/external`, this
directory is simply full of symbolic links.

The doctest fallback is fetched using `doctest.wrap`. Its header-only Meson
build definition is stored in `packagefiles/doctest/meson.build` and copied into
the downloaded source by the wrap's `patch_directory` setting. Keep this file
with the wrap so builds without a system doctest installation can configure.
