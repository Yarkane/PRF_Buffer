BEGIN {
	time = 0;
	buffered = 0;
	nlost = 0;
	nPackets = 0;
	totalDelay = 0;
}

{
	# collect data for the average number of buffered packets
	if($5 == "arrival") {
		delta_t = $2-time;
		buffered_avg += buffered*delta_t;
		time = $2;
		arrivals[$4] = time;
		buffered++;
		nPackets++;
	}
	else if($5 == "departure") {
		delta_t = $2-time;
		buffered_avg += buffered*delta_t;
		time = $2;
		totalDelay += time - arrivals[$4];
		buffered--;
	}
	else if($5 == "lost") {
		delta_t = $2-time;
		buffered_avg += buffered*delta_t;
		time = $2;
		nlost++;
		nPackets++;
	}
}

END {
	# output the average number of buffered packets
	printf("%12s %10.4f\n%12s %10.9f\n%12s %10.9f\n", "buffered", buffered_avg/time,"loss",nlost/nPackets,"delay",totalDelay/(nPackets-nloss));
}
