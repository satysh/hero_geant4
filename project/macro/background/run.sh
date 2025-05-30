#!/bin/bash

#root -l -q "background.C(100)" 

for ((i=0; i<37; i++)); do
	for ((j=1; j<8; j++)); do
		binlen=$((${i} * 7 + ${j}))
		echo ${binlen}    
		root -l -q "background.C(${binlen})" &
	done
	wait
done