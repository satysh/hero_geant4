#/bin/bash

#file_name="Fe_56_GeV_1000_events.root"
#root -l -q "pull_alpha_background.C(\"${file_name}\")" 1> /dev/null 2> /dev/stderr &
#sleep 5

file_name="helium3_30_GeV_1000_events.root"
root -l -q "pull_alpha_background.C(\"${file_name}\")" 
sleep 5

#file_name="proton_1_GeV_10000_events.root"
#root -l -q "pull_alpha_background.C(\"${file_name}\")" 1> /dev/null 2> /dev/stderr &
#sleep 5

#file_name="e_1_GeV_10000_events.root"
#root -l -q "pull_alpha_background.C(\"${file_name}\")" 1> /dev/null 2> /dev/stderr &
#sleep 5
