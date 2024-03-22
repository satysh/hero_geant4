#!/bin/bash

NTHREADS=9 # It must be (45 div NTHREADS) = 0
NBATCH=$((45 / NTHREADS - 1))

if [ -d output ]; then
	rm -f output/E0_*
	if [ -f output/calculate_alpha_counts_per_intervals_out.txt ]; then
		rm -fvi output/calculate_alpha_counts_per_intervals_out.txt
	fi
else
	mkdir output
fi

for BATCH_ID in $(seq 0 ${NBATCH}); do
	for THR in $(seq 0 $((NTHREADS - 1))); do
		INTERVAL_ID=$((BATCH_ID * NTHREADS + THR))
		root -l -q "calculate_alpha_counts_per_intervals.C(${INTERVAL_ID}, \"output/\", true)" &
	done
	wait
done

rm -f output/E0_*

