#!/usr/bin/bash
#
# This script is for CPEDocs: https://github.hpe.com/hpe/CPEDocs
#
# This script will collect documentation files that live in this repository and create a
# directory containing them. The newly created directory will have a structure compatible
# with the CPEDocs project and can be copied directly into CPEDocs/docs/debugging-tools,
# where the build system in CPEDocs will accept it.
#
# This script should be run from the directory it lives in.

# Stop immediately if anything unexpected happens
set -ex

echo "Creating output directory structure..."

OUTPUT_DIR="cpedocs_gdb4hpc"
mkdir -p "$OUTPUT_DIR/man"
mkdir -p "$OUTPUT_DIR/guides"

echo "Collecting files..."

# Collect index echo "  ./cpedocs_index_chunk.md -> $OUTPUT_DIR/"
cp ./cpedocs_index_chunk.md "$OUTPUT_DIR"

# Collect man pages
cp ../man/gdb4hpc.1.md "$OUTPUT_DIR/man"

cp -r ../guides/img "$OUTPUT_DIR/guides"

# Collect guides
for f in ../guides/*.md; do
  echo "  $f -> $OUTPUT_DIR/guides"
  cp "$f" "$OUTPUT_DIR/guides"
done

# We swap out some manpage headings like "NAME" to make the search experience in sphinx a little better.
confirm_and_replace() {
	# Relying on the fact that set -e was called at the top of the script for error checking
	grep "$1" "$3"
	sed -i "s/$1/$2/" "$3"
}

confirm_and_replace "# NAME" "# gdb4hpc Man Page" "$OUTPUT_DIR/man/gdb4hpc.1.md"

echo "Done. Output directory is $OUTPUT_DIR."
echo "Now copy $OUTPUT_DIR to /CPEDocs/doc/debugging-tools."
