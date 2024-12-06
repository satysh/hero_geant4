#!/bin/bash

NTHR=16
#E_ARRAY=($(seq 1 2)) # GeV
E_ARRAY=(10000 100000) # GeV

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

OUTDIR=tev

if [ -d ${OUTDIR} ]; then
    mv ../../output/*.root ${OUTDIR}
else
    mkdir ${OUTDIR}
    mv ../../output/*.root ${OUTDIR}
fi