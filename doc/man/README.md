# gdb4hpc man page generation

We use pandoc to generate a man page (roff format) from a markdown file. Don't
edit the .1 file by hand, edit the .md file and regenerate the .1 file.

`/external/cdst_build_library/doc/generate.sh` will automatically pass the correct arguments to pandoc and
install pandoc if you don't have it.
