#!/bin/bash

download_pandoc () {
    # link subject to change over time...
    local link=https://github.com/jgm/pandoc/releases/download/2.14.2/pandoc-2.14.2-linux-amd64.tar.gz
    wget $link
    echo $(basename "$link")
}

install_pandoc () {
    local prefix="/usr/local/"
    echo "Installing pandoc in $prefix"
    tar xvzf $1 --strip-components 1 -C $prefix
}

get_pandoc () {
    pushd /tmp
    local p=$(download_pandoc)
    install_pandoc /tmp/$p
    popd
}

if ! which pandoc; then
    while true; do
        read -p "pandoc not in PATH. Download and install it?" yn
        case $yn in
            [Yy]* ) get_pandoc; break;;
            [Nn]* ) exit;;
            * ) echo "Please answer yes or no.";;
        esac
    done
fi

echo "Generating..."
pandoc --standalone -r markdown+line_blocks+definition_lists -w man -o gdb4hpc.1 gdb4hpc.md
echo "Done."
