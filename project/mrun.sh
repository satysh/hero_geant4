#/bin/bash

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

./hero 1 1>out_1.txt 2>err_1.txt &
./hero 10 1>out_10.txt 2>err_10.txt &
./hero 20 1>out_20.txt 2>err_20.txt &
./hero 30 1>out_30.txt 2>err_30.txt &
./hero 40 1>out_40.txt 2>err_40.txt &
./hero 50 1>out_50.txt 2>err_50.txt &
./hero 60 1>out_60.txt 2>err_60.txt &
./hero 70 1>out_70.txt 2>err_70.txt &
./hero 80 1>out_80.txt 2>err_80.txt &
./hero 90 1>out_90.txt 2>err_90.txt &
./hero 100 1>out_100.txt 2>err_100.txt &
./hero 110 1>out_110.txt 2>err_110.txt &
./hero 120 1>out_120.txt 2>err_120.txt &
./hero 130 1>out_130.txt 2>err_130.txt &
./hero 140 1>out_140.txt 2>err_140.txt &
./hero 150 1>out_150.txt 2>err_150.txt &
./hero 160 1>out_160.txt 2>err_160.txt &
wait

cd -
if [ -d output ];then
    rm -vf output/*.root
    rm -vf output/*.txt
    mv ../build/*.root output/
else
    mkdir output
    mv ../build/*.root output/
fi
