#/bin/bash

NTHREADS=3

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
for THR in $(seq 1 ${NTHREADS}); do
    ./hero ${THR} 1> >(tee out_${THR}.txt ) 2> >(tee err_${THR}.txt) &
done
wait

cd -
if [ -d output ];then
    rm -vf output/*.root
    rm -vf output/*.txt
    mv ../build/*.root output/
    mv ../build/out*.txt output/
    mv ../build/err*.txt output/
else
    mkdir output
    mv ../build/*.root output/
    mv ../build/out.txt output/
    mv ../build/err.txt output/
fi
