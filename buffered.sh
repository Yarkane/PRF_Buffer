#! /bin/bash

# remove existing log files
rm -f buffered.log

# initialize lambda, mu and K
lambda=900
mu=1000
K=30

# for each simulation duration
for simtime in 0.05 0.1 0.2 0.3 0.4 0.5 1 2 5 10 20 50 100 200 300 500 700 1000; do

	# output the current simulation
	echo "Simulating [simtime " $simtime "/ lambda " $lambda "/ mu " $mu "/ K " $K "]"

	# run the simulation
	./buffer tracefile.log $simtime $lambda $mu $K

	# get the statistics
	awk -f postprocess.awk tracefile.log > temporary.log

	# store the averaged buffered packets in an overall
	# log, preceded by the relative simulation parameters
	awk -v t=$simtime -v l=$lambda -v m=$mu -v k=$K \
	    '$1 == "buffered" {printf("%10.2f %5d %5d %5d %10s\n", t, l, m, k, $2)}' \
	    temporary.log >> buffered.log

done

# remove temporary trace and log file
rm -f tracefile.log
rm -f temporary.log
