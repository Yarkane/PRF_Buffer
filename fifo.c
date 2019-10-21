/*
 * First In First Out buffer
 */

#include <stdio.h>
#include <stdlib.h>
#include "packet.h"
#include "fifo.h"

/*
 * get new FIFO buffer
 */
fifo_buffer_t *fifo_buffer_new() {
	fifo_buffer_t *new_buffer;

	/* allocate new buffer */
	if((new_buffer = (fifo_buffer_t *)malloc(sizeof(fifo_buffer_t))) == NULL) {
		fprintf(stderr, "FIFO buffer memory allocation error\n");
		exit(1);
	}
	/* initialize the new buffer */
	new_buffer->head = NULL;
	new_buffer->tail = NULL;
	new_buffer->size = 0;

	return(new_buffer);
}

/*
 * FIFO head insertion
 */
void fifo_buffer_in(fifo_buffer_t *buffer, packet_t *pkt) {
	/* error check on the passed packet */
	if(pkt == NULL) {
		fprintf(stderr, "Trying to insert NULL packet in FIFO buffer\n");
		return;
	}

	/* insertion in empty buffer */
	if(buffer->head == NULL) {
		buffer->head = pkt;
		buffer->tail = pkt;
	}
	/* insertion in non-empty buffer */
	else {
		pkt->next = buffer->head;
		buffer->head->prev = pkt;
		buffer->head = pkt;
	}

	/* increment buffer size */
	buffer->size++;

	return;
}

/*
 * FIFO tail extraction
 */
packet_t *fifo_buffer_out(fifo_buffer_t *buffer)
{
	packet_t *pkt;

	/* empty buffer */
	if(buffer->tail == NULL)
		return(NULL);

	/* extraction... */
	pkt = buffer->tail;
	buffer->tail = pkt->prev;

	/* ...of the only packet */
	if(buffer->tail == NULL)
		buffer->head = NULL;
	/* ...of one packet */
	else {
		buffer->tail->next = NULL;
		pkt->prev = NULL;
	}

	/* decrement buffer size */
	buffer->size--;

	return(pkt);
}

/*
 * debug: dump content of a FIFO buffer
 */
void fifo_buffer_dump(fifo_buffer_t *buffer) {
	packet_t *pkt;
	int i;

	fprintf(stderr, "----- buffer_dump -----\n");

	pkt = buffer->head;
	i = 0;
	while(pkt != NULL) {
		fprintf(stderr, "%10d %10d\n", i, pkt->uid);
		pkt = pkt->next;
		i++;
	}

	fprintf(stderr, "-----------------------\n");

	return;
}


