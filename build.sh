#/bin/bash
REPOROOT="$(pwd)"

mkdir -p build
cd build
cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DREPOROOT=$REPOROOT ..
ninja -j9
cd ..