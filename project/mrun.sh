#!/bin/bash

# ./tmp.sh ENERGY NEVENTS_IN_RUN NTHREADS NBATCH
# result NEVENTS= NEVENTS_IN_RUN * NTHREADS * NBATCH
./tmp.sh 20000 5 16 22
./tmp.sh 40000 5 16 22			
./tmp.sh 60000 5 16 22
./tmp.sh 80000 5 16 22 