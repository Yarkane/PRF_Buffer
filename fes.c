/*
 * Future Event Set list - header
 */

#include <stdio.h>
#include <stdlib.h>
#include "fes.h"

/*
 * global variables
 */
event_t *fes_head = NULL;      /* head of FES list */
event_t *fes_tail = NULL;      /* tail of FES list */
event_t *fes_freelist = NULL;  /* single-linked list of
                                  new or released events */

/*
 * get new event
 */
event_t *event_new() {
	event_t *evt;
	int i;

	/* no free events available, allocate FES_FREELIST_GROWTH new events */
	if(fes_freelist == NULL) {
		for(i=0; i<FES_FREELIST_GROWTH; i++) {
			/* allocate i-th new event */
			if((evt = (event_t *)malloc(sizeof(event_t))) == NULL) {
				fprintf(stderr, "FES freelist memory allocation error\n");
				exit(1);
			}
			/* initialize new event */
			evt->next = NULL;
			evt->prev = NULL;
			evt->type = RELEASE;
			evt->time = -1;
			/* insert new event at the head of free event list */
			if(fes_freelist != NULL)
				evt->next = fes_freelist;
			fes_freelist = evt;
		}
	}

	/* get first free event available */
	evt = fes_freelist;
	fes_freelist = evt->next;
	evt->next = NULL;

	return(evt);
}

/*
 * release event
 */
void event_release(event_t *evt)
{
	/* reset event */
	evt->next = NULL;
	evt->prev = NULL;
	evt->type = RELEASE;
	evt->time = -1;
	/* insert new event at the head of free event list */
	if(fes_freelist != NULL)
		evt->next = fes_freelist;
	fes_freelist = evt;

	return;
}

/*
 * event FES insertion
 */
void fes_insert(event_t *evt) {
	event_t *it;

	/* error check on the passed element */
	if(evt == NULL) {
		fprintf(stderr, "Trying to insert NULL element in FES list\n");
		return;
	}

	/* insertion in empty FES */
	if(fes_head == NULL) {
		fes_head = evt;
		fes_tail = evt;
		return;
	}

	/* insertion at the tail of the FES list (farthest event in time) */
	if(evt->time >= fes_tail->time) {
		evt->prev = fes_tail;
		fes_tail->next = evt;
		fes_tail = evt;
	}
	/* insertion within the FES list */
	else {
		/* look for position */
		it = fes_tail;
		while(it != NULL && evt->time < it->time)
			it = it->prev;
		/* insertion at the head of the list (nearest event in time) */
		if(it == NULL) {
			evt->next = fes_head;
			fes_head->prev = evt;
			fes_head = evt;
		}
		/* insertion in the middle of the list */
		else {
			evt->next = it->next;
			evt->prev = it;
			it->next->prev = evt;
			it->next = evt;
		}
	}

	return;
}

/*
 * next event extraction from FES
 */
event_t *fes_get() {
	event_t *evt;

	/* empty queue */
	if(fes_head == NULL)
		return(NULL);

	/* extraction... */
	evt = fes_head;
	fes_head = evt->next;

	/* ...of the only element */
	if(fes_head == NULL)
		fes_head = NULL;
	/* ...of one element */
	else {
		fes_head->prev = NULL;
		evt->next = NULL;
	}

	return(evt);
}

/*
 *  assist function: event scheduling
 */
void schedule(event_type_t type, double time) {
	event_t *evt;

	evt = event_new();
	evt->type = type;
	evt->time = time;
	fes_insert(evt);

	return;
}

/*
 *  assist function: event extraction
 */
event_t *get() {
	return(fes_get());
}

/*
 * debug: dump content of FES list
 */
void fes_dump() {
	event_t *evt;
	int i;

	fprintf(stderr, "--------- fes_dump ---------\n");

	evt = fes_head;
	i = 0;
	while(evt != NULL) {
		fprintf(stderr, "%10d %12.4f %4d\n", i, evt->time, evt->type);
		evt = evt->next;
		i++;
	}

	fprintf(stderr, "----------------------------\n");

	return;
}

/*
 * debug: dump content of event freelist
 */
void fes_freelist_dump() {
	event_t *evt;
	int i;

	fprintf(stderr, "-------- event_dump --------\n");

	evt = fes_freelist;
	i = 0;
	while(evt != NULL) {
		fprintf(stderr, "%10d %12.4f %4d\n", i, evt->time, evt->type);
		evt = evt->next;
		i++;
	}

	fprintf(stderr, "----------------------------\n");

	return;
}


