#include "types.h"

window *firstwp;	/* first winow		*/
window *curwp;		/* Current window   */
buffer *curbp;		/* Current buffer   */

/*
 * kill the program
 */
void die(const char *s) {
	ttclose();
	fprintf(stderr, "%s\n", s);
	exit(2);
}
