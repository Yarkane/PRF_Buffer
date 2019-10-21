/*
 * Future Event Set list - header
 */

#ifndef FES_H
#define FES_H

#define FES_FREELIST_GROWTH 10  /* free event list size increment */

/*
 * event types
 */
typedef enum {RELEASE, ARRIVAL, DEPARTURE} event_type_t;

/*
 * event
 */
typedef struct event_type event_t;
struct event_type {
	event_t *next;      /* next FES list element */
	event_t *prev;      /* previous FES list element */
	event_type_t type;  /* event type */
	double time;        /* time of event */

	/* attributes that depend on the event type */
};

/*
 * prototypes
 */
event_t *event_new();                 /* get new event */
void event_release(event_t *);        /* release event */
void fes_insert(event_t *);           /* event insertion in FES */
event_t *fes_get();                   /* next event extraction from FES */
void schedule(event_type_t, double);  /* event scheduling assist function */

/*
 * debug
 */
void fes_dump();           /* dump content of FES list */
void fes_freelist_dump();  /* dump content of event freelist */

#endif


