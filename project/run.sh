#!/bin/bash

NTHR=16
#E_ARRAY=($(seq 1 15)) # GeV
E_ARRAY=(6 12 25 50 100) # GeV

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
make -j${NTHR}

time (
    for ((i=0; i<${#E_ARRAY[@]}; i++)); do
        ./hero ${E_ARRAY[i]} ${NTHR} 1> >(tee out.txt ) 2> >(tee err.txt)
    done
)

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

cd macro/scint_light/

if [ -d electron ]; then
    mv ../../output/*.root electron/
else
    mkdir electron/
    mv ../../output/*.root electron/
fi