#!/bin/bash

if [ -d output ];then
    rm -fv output/full_edeps.txt
else
    mkdir output
fi

fname=_tmp_file.txt
ls input/ > ${fname}

root -l -q "energy_release.C(\"${fname}\")" 2> output/full_edeps.txt
wait

rm -fv ${fname}
echo "DONE"