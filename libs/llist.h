#ifndef _LLIST_H
# define _LLIST_H

/*
 *	Linked list library
 * 	parsa mahmoudy
 */

#define L_LINK(type) struct { type *next; type *prev; }

#define L_LINK_NEXT(o) ((o)->link.next)
#define L_LINK_PREV(o) ((o)->link.prev)
#define L_LINK_SNEXT(s,d) ((s)->link.next = d)
#define L_LINK_SPREV(s,d) ((s)->link.prev = d)

#define L_LINK_INSERT(l,n) { \
	if (L_LINK_NEXT(l) != NULL) { \
		L_LINK_SPREV(L_LINK_NEXT(l),n); \
		L_LINK_SNEXT(n,L_LINK_NEXT(l)); \
	} \
	L_LINK_SPREV(n,l); \
	L_LINK_SNEXT(l,n); \
}

#define L_LINK_GET_LAST(o) { \
	for (; o != NULL; o = L_LINK_NEXT(o)); \
}

#define L_LINK_REMOVE(o) { \
	int have_next = L_LINK_NEXT(o) != NULL; \
	int have_prev = L_LINK_PREV(o) != NULL; \
	if (have_next) { \
		if (have_prev) {\
			L_LINK_SNEXT(L_LINK_PREV(o),L_LINK_NEXT(o)); \
			L_LINK_SPREV(L_LINK_NEXT(o),L_LINK_PREV(o)); \
		} else \
			L_LINK_SPREV(L_LINK_NEXT(o),NULL); \
	} \
	if (have_prev && !have_next) \
		L_LINK_SNEXT(L_LINK_PREV(o),NULL); \
	L_LINK_SNEXT(o,NULL); \
	L_LINK_SPREV(o,NULL); \
}

#endif
