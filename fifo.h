/*
 * First In First Out buffer - header
 */

#ifndef FIFO_BUFFER_H
#define FIFO_BUFFER_H

/*
 * FIFO buffer
 */
typedef struct fifo_buffer_type fifo_buffer_t;
struct fifo_buffer_type {
	packet_t *head;  /* head of FIFO buffer */
	packet_t *tail;  /* tail of FIFO buffer */
	int size;        /* size of FIFO buffer */
};

/*
 * prototypes
 */
fifo_buffer_t *fifo_buffer_new();                  /* get new FIFO buffer */
void fifo_buffer_in(fifo_buffer_t *, packet_t *);  /* FIFO head insertion */
packet_t *fifo_buffer_out(fifo_buffer_t *);			   /* FIFO tail extraction */

/*
 * debug
 */
void fifo_buffer_dump(fifo_buffer_t *);  /* dump content of FIFO buffer */

#endif


