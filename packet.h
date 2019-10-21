/*
 * packet, the entity that transits in the buffer - header
 */

#ifndef PACKET_H
#define PACKET_H

#define PACKET_FREELIST_GROWTH 10  /* free packet list size increment */

/*
 * entity
 */
typedef struct packet_type packet_t;
struct packet_type {
	packet_t *next;       /* next pointer, used for queuing */
	packet_t *prev;       /* prev pointer, used for queuing */
	unsigned int uid;     /* packet unique identifier */
};

/*
 * prototypes
 */
packet_t *packet_new();           /* get new packet */
void packet_release(packet_t *);  /* release packet */

/*
 * debug
 */
void packet_freelist_dump();  /* dump content of packet freelist */

#endif

