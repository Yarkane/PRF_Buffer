BEGIN {
	time = 0;
	buffered = 0;
}

{
	# collect data for the average number of buffered packets
	if($5 == "arrival") {
		delta_t = $2-time;
		buffered_avg += buffered*delta_t;
		time = $2;
		buffered++;
	}
	else if($5 == "departure") {
		delta_t = $2-time;
		buffered_avg += buffered*delta_t;
		time = $2;
		buffered--;
	}
}

END {
	# output the average number of buffered packets
	printf("%12s %10.4f\n\n", "buffered", buffered_avg/time);
}


