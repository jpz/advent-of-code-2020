#!/bin/bash

# get full path of current directory
# (source: https://stackoverflow.com/a/246128)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

for file in "$DIR/build/bin/"*
do
    echo "$(basename ${file}):"
    (cd "${DIR}/data"; "$file")
    echo "" #-----------"; echo ""
done | tee outputs.txt
