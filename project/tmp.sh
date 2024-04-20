#/bin/bash

if [ -n "$1" ]
then
    ENERGY=$1
    NEVENTS_IN_RUN=$2
    NTHREADS=$3
    NBATCH=$4
else
echo "No parameters found. "
exit
fi

BOPT="wb"
echo ENERGY=${ENERGY}
echo NEVENTS_IN_RUN=${NEVENTS_IN_RUN}
echo NTHREADS=${NTHREADS}
echo NBATCH=${NBATCH}
echo BOPT=${BOPT}
# result NEVENTS= NTHREADS * NEVENTS_IN_RUN * NBATCH

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

for BATCH_ID in $(seq 0 $((NBATCH - 1))); do
    #echo "BATCH_ID="${BATCH_ID}
    for THR in $(seq 0 $((NTHREADS - 1))); do
        INTERVAL_ID=$((BATCH_ID * NTHREADS + THR))
        echo INTERVAL_ID=${INTERVAL_ID}
        ./hero ${INTERVAL_ID} ${ENERGY} ${NEVENTS_IN_RUN} ${BOPT} 1> >(tee out_${INTERVAL_ID}.txt ) 2> >(tee err_${INTERVAL_ID}.txt) &
    done
    wait
    
    mv *.root ../project/output
    rm -fv *.txt
    cd ../project/macro/diff_r/
    wait
    root -l -q "sum_edep_distribution_calc.C(${BATCH_ID}, ${ENERGY})" 
    cd -
    rm -fv ../project/output/*.root
done
wait

pwd
cd ../project/macro/diff_r

if [[ ${BOPT} = "wb" ]];then
    mv sum_edep_distribution_calc.root wb_${ENERGY}_GeV_sum_edep_distribution_calc.root
else
    mv sum_edep_distribution_calc.root ${ENERGY}_GeV_sum_edep_distribution_calc.root
fi