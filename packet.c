/*
 * packet, the entity that transits in the buffer
 */

#include <stdio.h>
#include <stdlib.h>
#include "packet.h"

/*
 * global variables
 */
unsigned int packet_uid_counter = 1;  /* unique identifier of next packet */
packet_t *packet_freelist = NULL;     /* single-linked list of
                                         new or released packets */

/*
 * get new packet
 */
packet_t *packet_new() {
	packet_t *pkt;
	int i;

	/* no free packets available, allocate PACKET_FREELIST_GROWTH new packets */
	if(packet_freelist == NULL) {
		for(i=0; i<PACKET_FREELIST_GROWTH; i++) {
			/* allocate i-th new packet */
			if((pkt = (packet_t *)malloc(sizeof(packet_t))) == NULL) {
				fprintf(stderr, "Packet memory allocation error\n");
				exit(1);
			}
			/* initialize new packet */
			pkt->next = NULL;
			pkt->prev = NULL;
			pkt->uid = 0;
			/* insert new packet at the head of free packet list */
			if(packet_freelist != NULL)
				pkt->next = packet_freelist;
			packet_freelist = pkt;
		}
	}

	/* get first free packet available */
	pkt = packet_freelist;
	packet_freelist = pkt->next;
	pkt->next = NULL;
	pkt->uid = packet_uid_counter++;

	return(pkt);
}

/*
 * release packet
 */
void packet_release(packet_t *pkt)
{
	/* reset packet */
	pkt->next = NULL;
	pkt->prev = NULL;
	pkt->uid = 0;
	/* insert reset packet at the head of free packet list */
	if(packet_freelist != NULL)
		pkt->next = packet_freelist;
	packet_freelist = pkt;

	return;
}

/*
 * debug: dump content the packet freelist
 */
void packet_freelist_dump() {
	packet_t *pkt;
	int i;

	fprintf(stderr, "----- packet_dump -----\n");

	pkt = packet_freelist;
	i = 0;
	while(pkt != NULL) {
		fprintf(stderr, "%10d %10d\n", i, pkt->uid);
		pkt = pkt->next;
		i++;
	}

	fprintf(stderr, "-----------------------\n");

	return;
}


