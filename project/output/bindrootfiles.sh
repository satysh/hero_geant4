#/bin/bash

rm -fv hero*.root
ls *.root > .files_list.txt
wait
root -l -q bindrootfiles.C
wait
rm -fv .files_list.txt
