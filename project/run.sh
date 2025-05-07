#!/bin/bash

NTHR=14
NEVENTS=1000;
BACKGROUNDPERIOD=0
#E_ARRAY=($(seq 1 1)) # GeV
#E_ARRAY=(1000 2500 5000 7500 10000 25000 50000 75000 100000) # GeV
E_ARRAY=(1000 2500 5000 7500 10000) # GeV

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
    RANDOM_STATE=123456789
    BOPT=b
    for ((i=0; i<${#E_ARRAY[@]}; i++)); do
        ./hero ${E_ARRAY[i]} ${NTHR} ${RANDOM_STATE} ${BOPT} ${NEVENTS}  1> >(tee out.txt ) 2> >(tee err.txt)
        #./hero background ${NTHR} ${RANDOM_STATE} ${BOPT} ${NEVENTS} ${BACKGROUNDPERIOD} 1> >(tee out.txt ) 2> >(tee err.txt)
    done

    RANDOM_STATE=123456789
    BOPT=-b
    for ((i=0; i<${#E_ARRAY[@]}; i++)); do
        ./hero ${E_ARRAY[i]} ${NTHR} ${RANDOM_STATE} ${BOPT} ${NEVENTS}  1> >(tee out.txt ) 2> >(tee err.txt)
        #./hero background ${NTHR} ${RANDOM_STATE} ${BOPT} ${NEVENTS} ${BACKGROUNDPERIOD} 1> >(tee out.txt ) 2> >(tee err.txt)
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

OUTDIR=single_energy

if [ -d ${OUTDIR} ]; then
    mv ../../output/*.root ${OUTDIR}
else
    mkdir ${OUTDIR}
    mv ../../output/*.root ${OUTDIR}
fi