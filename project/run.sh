#/bin/bash

rm -rfv ../build/*
cd ../build
cmake ../project
wait
make -j3
wait
./hero
