#/bin/bash

if [ -d out_diff_e/ ];then
    rm -fv out_diff_e/*
else
    mkdir out_diff_e
fi

cd ../build
make -j3
wait

particle=1
energy=10
nevents=100000
out_file_name=particle_${particle}_${energy}_GeV_${nevents}_events.root

./hero ${particle} ${energy} ${nevents} ${out_file_name}
wait

mv ${out_file_name} ../project/out_diff_e

particle=1
energy=100
nevents=10000
out_file_name=particle_${particle}_${energy}_GeV_${nevents}_events.root

./hero ${particle} ${energy} ${nevents} ${out_file_name}
wait

mv ${out_file_name} ../project/out_diff_e

particle=1
energy=1000
nevents=1000
out_file_name=particle_${particle}_${energy}_GeV_${nevents}_events.root

./hero ${particle} ${energy} ${nevents} ${out_file_name}
wait

mv ${out_file_name} ../project/out_diff_e

