#ifndef _VT100_H
# define _VT100_H

/*
 *	control vt100 screen 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define NROW    24			/* Screen size.                 	*/
#define NCOL    80			/* edit if you want to.         	*/
#define	MARGIN	8			/* size of minimim margin and   	*/
#define	SCRSIZ	64			/* scroll size for extended lines   */
#define	NPAUSE	100			/* n times thru update to pause 	*/
#define BIAS    0x20		/* origin 0 coordinate bias.    	*/
#define ESC     0x1B		/* ESC character.               	*/
#define BEL     0x07		/* ascii bell character         	*/

extern struct terminal term;

/*
 *	vt100 funcitons :)
 */
void vt100move();
void vt100eeol();
void vt100eeop();
void vt100beep();
void vt100mnl();
void vt100mpl();
void vt100mfw();
void vt100mbw();

#endif
