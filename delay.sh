#! /bin/bash

# remove existing log files
rm -f delay.log

# initialize lambda, mu and K
mu=1000
simtime=100

# for each simulation duration
for lambda in 500 700 900 1000 1100 1300 ; do
	for K in 1 5 10 15 20 25 30 40 50 70 100 ; do

		# output the current simulation
		echo "Simulating [simtime " $simtime "/ lambda " $lambda "/ mu " $mu "/ K " $K "]"

		# run the simulation
		./buffer tracefile.log $simtime $lambda $mu $K

		# get the statistics
		awk -f postprocess.awk tracefile.log > temporary.log

		# store the averaged delay packets in an overall
		# log, preceded by the relative simulation parameters
		awk -v t=$simtime -v l=$lambda -v m=$mu -v k=$K \
		    '$1 == "delay" {printf("%10.2f %5d %5d %5d %10s\n", t, l, m, k, $2)}' \
		    temporary.log >> delay_$lambda.log
	done
done

# remove temporary trace and log file
rm -f tracefile.log
rm -f temporary.log
