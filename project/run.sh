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

# max_percent / n_jobs - должно быть целым числом
max_percent=10
n_jobs=5

for i in $(seq 0 $((max_percent / n_jobs - 1))); do
    for j in $(seq 0 $((n_jobs - 1))); do
        percent=$((i * n_jobs + j + 1))
        ./hero ${percent} ${percent} 1> >(tee out.txt ) 2> >(tee err.txt) &
    done
    wait
done

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

mv output/*.root macro/dampe_1_year/data/