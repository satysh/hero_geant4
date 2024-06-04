#!/bin/bash

if [[ -d archive_40 ]]; then
	echo "archive_40 is found!"
else
	mkdir archive_40
fi
while [[ true ]]; do
	if [[ $(ls ../../output/ | wc -l) = 2 ]]; then
		if [[ $(ls tmp_output/ | wc -l) > 0 ]]; then
		    mv -v tmp_output/* archive_40
		fi
	else
		echo "wait" 	
	fi 
	sleep 1
done