/*
 * Buffer with finite size and exponential arrivals
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "packet.h"
#include "fes.h"
#include "fifo.h"
/* externs function declarations */
#include "functions.h"

#define FILENAMELENGTH 512
//#define DEBUG

/* global variables */
long seed_lambda = 14895743;
long seed_mu = 65447236;
double now = -1;
fifo_buffer_t *buffer;
FILE *tracefile;
double lambda, mu, k;

double nLost = 0;
event_t *nextEvent;

/* process arrival */
void callback_arrival() {
	packet_t *pkt;
	pkt = packet_new();

	//Place disponible ??
	if(k > buffer->size){
		fifo_buffer_in(buffer, pkt);
		//si c'est le premier paquet du buffer : schedule le departure
		if(buffer->size == 1) {
			schedule(DEPARTURE, now+exponential(mu, &seed_mu));
		}
		/* tracing */
		fprintf(tracefile, "at %.6f pkt %d arrival\n", now, pkt->uid);
	}else{
		nLost++;
		/* tracing */
		fprintf(tracefile, "at %.6f pkt %d lost\n", now, pkt->uid);
	}
	//On schedule la prochaine arrivée
	schedule(ARRIVAL,now+exponential(lambda, &seed_lambda));
}

/* process departure */
void callback_departure() {
	packet_t *pkt;

	/* get served packet from the FIFO buffer */
	pkt = fifo_buffer_out(buffer);
	/* start serving the following packet in the FIFO buffer */
	if(buffer->size > 0) {
		schedule(DEPARTURE, now+exponential(mu, &seed_mu));
	}

	/* tracing */
	fprintf(tracefile, "at %.6f pkt %d departure\n", now, pkt->uid);

	/* release processed packet */
	packet_release(pkt);

	return;
}

int main(int argc, const char* argv[]) {
  /*
	event_t *evt;
  */
	char tracefilename[FILENAMELENGTH];
	double stop;

	/* process commmand line */
	if(argc != 6) {
		fprintf(stderr, "\n%8s <tracefile> <stop> "
		                "<arrival_rate> <service_rate> "
		                "<queue_size>\n\n",
		        argv[0]);
		fprintf(stderr, "         <tracefile>    : output trace file\n"
		                "         <stop>         : simulation duration (s)\n"
		                "         <arrival_rate> : arrival rate (packet/s)\n"
		                "         <service_rate> : service rate (packet/s)\n"
		                "         <queue_size>   : FIFO queue size (packet)\n\n");
		return(0);
	}
	strcpy(tracefilename, argv[1]);
	stop = atof(argv[2]);
	lambda = atof(argv[3]);
	mu = atof(argv[4]);
	k = atoi(argv[5]);

	double nPackets = stop*lambda;

	/* open output trace file */
	tracefile = fopen(tracefilename, "w");
	if(tracefile == NULL) {
		fprintf(stderr, "Could not open output trace file '%s'\n", tracefilename);
		exit(1);
	}
	setbuf(tracefile, NULL);

	/* allocate FIFO buffer */
	buffer = fifo_buffer_new();

	/* schedule first arrival */
	schedule(ARRIVAL, exponential(lambda, &seed_lambda));

	/* event loop */
	while(now < stop) {

#ifdef DEBUG //================================================================
		fes_dump();
		fifo_buffer_dump(buffer);
#endif //======================================================================

		nextEvent = fes_get();
		now = nextEvent->time;
		if(nextEvent->type == ARRIVAL) callback_arrival();
		else if (nextEvent->type == DEPARTURE) callback_departure();
		else printf("wtf ?\n");


	}

	printf("Packets loss = %f\n",nLost);
	printf("All packets = %f\n",nPackets);
	return(0);
}
