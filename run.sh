#!/bin/bash

# get full path of current directory
# (source: https://stackoverflow.com/a/246128)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd "$DIR/data"

for filename in $(ls "$DIR/build/bin/")
do
    #if [[ $filename != day15b ]]
    if [[ 1 == 1 ]]
    then
        echo $filename
        "$DIR/build/bin/$filename"
        echo ""
    fi
done | tee ../outputs.txt
