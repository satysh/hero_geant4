#/bin/bash

NTHREADS=16 # 
NBATCH=10
echo NBATCH=${NBATCH}
# result NTHREADS * nevents_in_run * NBATCH

if [ -d output ];then
    rm -vf output/*.root
    rm -vf output/*.txt
    rm -vf macro/diff_r/sum_edep_distribution_calc.root
else
    mkdir output
fi

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
sleep 5
make -j${NTHREADS}
wait
sleep 5

for BATCH_ID in $(seq 0 ${NBATCH}); do
    #echo "BATCH_ID="${BATCH_ID}
    for THR in $(seq 0 $((NTHREADS - 1))); do
        INTERVAL_ID=$((BATCH_ID * NTHREADS + THR))
        echo INTERVAL_ID=${INTERVAL_ID}
        ./hero ${INTERVAL_ID} 1> >(tee out_${INTERVAL_ID}.txt ) 2> >(tee err_${INTERVAL_ID}.txt) &
    done
    wait
    
    mv *.root ../project/output
    rm -fv *.txt
    cd ../project/macro/diff_r/
    wait

    root -l -q "sum_edep_distribution_calc.C(${BATCH_ID})" &
    wait
    cd -
    rm -fv ../project/output/*.root
done
wait

