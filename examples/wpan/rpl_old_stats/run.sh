#!/bin/bash


Start_I="3 5 7 9 11"
Start_K="1 2 3 5 7 9 11 13 15"
Max_I="20"
parent_set="1"
#reset_time="8280"
trickle_types="normal trickleM"


reset_time="100663 1035"
#Start_I="3"
#Start_K="1"
#parent_set="1"

#~/Simulators/omnet/omnetpp-4.2.1/bin/opp_runall -j2 ../prova -f omnetpp.ini -l ~/Simulators/omnet/omnetpp-4.2.1/workspace/rpl_repo/inet/out/gcc-debug/src/libinet.so -n ../../MiXiM/examples:../../MiXiM/src:../../MiXiM/tests:../src:../../inet/examples:../../inet/src:. -u Cmdenv -r 1..10

for r in $reset_time; do

	for I in $Start_I; do

	  for k in $Start_K; do
	      for p in $parent_set; do
		  for m in $Max_I; do
		    for t in $trickle_types; do
			mkdir -p ../../data_uniform_100/data_$t
			mkdir -p /mnt/ram/uniform_100
			if [ ! -d "../../data_uniform_100/data_${t}/${I}_${k}_${p}_${r}_${m}" ]; then
			    echo **.minInt = $I > params.ini
			    echo **.maxDoublings = $m >> params.ini
			    echo **.redundancy = $k >> params.ini
			    echo **.parent_set_size = $p >> params.ini
			    echo **.trickleType = \"${t}\" >> params.ini
			    echo sim-time-limit = ${r}s >> params.ini
			    echo **.directory = \"/mnt/ram/uniform_100/\" >> params.ini
			    rm -rf results /mnt/ram/uniform_100/*.log
			    ./omnetpp_bin/opp_runall -j 4 ./omnetpp_bin/opp_run_release -r 1..100 -u Cmdenv  -n ../..:../../../src -l ../../../src/inet omnetpp.ini

			    mkdir ../../data_uniform_100/data_${t}/${I}_${k}_${p}_${r}_${m}
			    mv /mnt/ram/uniform_100/*.log ../../data_uniform_100/data_${t}/${I}_${k}_${p}_${r}_${m}
			fi
		    done
		  done
		done
	    done    
	done
done


exit 0
