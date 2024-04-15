#/bin/bash

NTHREADS=16 # It must be (45 div NTHREADS) = 0
NRUNS=100
NBATCH=$((NRUNS / NTHREADS - 1))
echo NBATCH=${NBATCH}

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
make -j${NTHREADS}
wait

for BATCH_ID in $(seq 0 ${NBATCH}); do
    #echo "BATCH_ID="${BATCH_ID}
    for THR in $(seq 0 $((NTHREADS - 1))); do
        INTERVAL_ID=$((BATCH_ID * NTHREADS + THR))
        echo INTERVAL_ID=${INTERVAL_ID}
        ./hero ${INTERVAL_ID} 1> >(tee out_${INTERVAL_ID}.txt ) 2> >(tee err_${INTERVAL_ID}.txt) &
    done
    wait
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
