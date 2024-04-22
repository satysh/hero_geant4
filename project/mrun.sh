#!/bin/bash

# ./tmp.sh ENERGY NEVENTS_IN_RUN NTHREADS NBATCH
# result NEVENTS= NEVENTS_IN_RUN * NTHREADS * NBATCH
./tmp.sh 20 100 16 11
./tmp.sh 40 100 16 11			
./tmp.sh 60 100 16 11
./tmp.sh 80 100 16 11