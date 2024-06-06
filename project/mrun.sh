#!/bin/bash

# ./tmp.sh ENERGY NEVENTS_IN_RUN NTHREADS NBATCH SEED_BEGIN
# result NEVENTS= NEVENTS_IN_RUN * NTHREADS * NBATCH
# SEED_BEGIN is a number of last event. By def. It is 0.
./tmp.sh 20000 1 16 110 0
./tmp.sh 40000 1 16 110	0		
./tmp.sh 60000 1 16 110 0
./tmp.sh 80000 1 16 110 0