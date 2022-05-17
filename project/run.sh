#/bin/bash

if [ -d ../build ];then
    echo "../build was found!"
else
    mkdir ../build
    cd ../build
    cmake ../project
    wait
    cd -
fi
cd ../build
make -j3
wait
./hero
