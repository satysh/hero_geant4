#/bin/bash

NTHREADS=3
STARTENERGY=1
NENERGIES=70

rm -fv output/*.root
rm -fv output/*.txt
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


for EI in $(seq ${STARTENERGY} ${NENERGIES}); do
    for THR in $(seq 1 ${NTHREADS}); do
        nice ./hero ${THR} ${EI} ${NTHREADS} 1> >(tee out_${THR}.txt ) 2> >(tee err_${THR}.txt) &
    done
    wait
    rm -fv *.txt
    rm -fv ../project/output/*.root
    mv -fv *.root ../project/output
    cd ../project/output
    ./bindrootfiles.sh
    wait
    mv hero.root ../archive/hero_${EI}.root
    cd -
done
wait
