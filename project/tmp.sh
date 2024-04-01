#!/bin/bash

if [ -d ../build ];then
    echo "../build was found!"
    rm -fv ../build/*.root
    rm -fv ../build/*.txt
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

./hero 30 1> >(tee out.txt ) 2> >(tee err.txt) &
./hero 50 &
./hero 62 &
./hero 80 &
./hero 100 &
./hero 125 &
./hero 250 &
./hero 500 &
./hero 1000 1> >(tee out_1000.txt ) 2> >(tee err_1000.txt)
wait

cd -
if [ -d output ];then
    rm -vf output/*.root
    rm -vf output/*.txt
    mv ../build/*.root output/
    mv ../build/out.txt output/
    mv ../build/err.txt output/
else
    mkdir output
    mv ../build/*.root output/
    mv ../build/out.txt output/
    mv ../build/err.txt output/
fi