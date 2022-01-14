/*
 *	Manager buffers
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

/*
 *	alloc and init new buffer with given name and path , also we can set
 *	modes and flags
 */
buffer *init_buffer(char *filename,short modes,short flags)
{
	buffer *bf;
	if (!(bf = malloc(sizeof(buffer))))
		die("malloc buffer");

	if (filename != NULL && strlen(filename) < NFILEN)
		set_buffer_name(filename);
	else 
		lv_strncpy(bf->bname,NO_NAME_BUFFER,strlen(NO_NAME_BUFFER));
	/* when a buffer initialized , we have set lock mode for it , if we don't want to be locked ,we must specify it at modes input */
	bf->modes |= MDLOCK;
	bf->modes |= modes;
	bf->flags = FFULLS;
	bf->flags = flags;
	bf->nrow = statusbar_start_offset - buffers_start_offset - 1;
	bf->loffset = bf->coffset = bf->dirty = bf->clindex = 0;
	bf->flags |= FREDRW;
	bf->mtop = buffers_start_offset;
	bf->mleft = 1;
	bf->linenm = false;
	return bf;
}

/*
 *	will search in buffers and and set founded buffer as active buffer
 *	if no any buffer founded , will return false :)
 */
int set_buffer_by_name(char *name)
{
	for (window *wp = firstwp;wp != NULL;wp = L_LINK_NEXT(wp)) {
		for (buffer *bf = wp->fbuffer;bf != NULL;bf = L_LINK_NEXT(bf)) {
			if (strcmp(name,bf->bname) == 0) {
				activate_window(wp);
				change_current_buffer(bf);
				return true;
			}
		}
	}
	return false;
}

/*
 *	set buffer name and path by given string (path)
 *	also we will call load_syntax_for_buffer
 */
void set_buffer_name(char *path)
{
	memcpy(curbp->fname,path,strlen(path));
	get_filename_of_path(curbp->bname,path);
	load_syntax_for_buffer();
}

/*
 *	if buffer name is emtpy so we will return to exit function
 *	otherwise we will check file extension to get intended language name
 *	them load syntax file associated with that language
 */
void load_syntax_for_buffer()
{
	if (curbp->bname == NULL)
		return;
	if (string_endwith(curbp->bname,".c") || string_endwith(curbp->bname,".h")) {
		curbp->filetype = strdup("c");
	} else if (string_endwith(curbp->bname,".cs"))
		curbp->filetype = strdup("cs");
	load_syntax(curbp->filetype);
}

/*
 *	works that we need to do when buffer changed
 */
void buffer_changed()
{
	curbp->flags |= FREDRW;
	curbp->dirty++;
}

/*
 *	actully it will toggle line number for buffer :)
 */
int toggle_linenumber()
{
	curbp->linenm = !curbp->linenm;
	curbp->flags |= FREDRW;
	return update_linenumber_padding();
}

/*
 *	append buffer into given window
 */
void append_buffer(window *win,buffer *bf)
{
	if (win == NULL)
		die("NO any window");
	/* if currwp buffers null , so it's first one , otherwise add to last buffers next */
	if (win->fbuffer == NULL) {
		win->fbuffer = bf;
		L_LINK_SNEXT(bf,NULL);
	}
	else {
		L_LINK_INSERT(get_last_buffer(win),bf);
	}
	win->bcount++;
}

/*
 *	return last line of given buffer
 *	if given buffer is NULL we will get last line of curbp
 */
line *get_last_line(buffer *bf)
{
	/* set to curwp if wp is null */
	buffer *buff = bf != NULL ? bf : curbp;
	line *l = buff->fline;
	for (; l != NULL;l = L_LINK_NEXT(l));
	return l;
}

/*
 *	set given mode for buffer
 *	we have to check given mode , for example if that was insert mode
 *	we have to turn lock and visual mode to off and ...
 */
void set_mode_for_buffer(int mode)
{
	if (mode == MDLOCK) {
		usmode(curbp,MDVISL);
		usmode(curbp,MDINST);
		usmode(curbp,MDCMMD);
		stmode(curbp,MDLOCK);
	} else if (mode == MDVISL) {
		usmode(curbp,MDLOCK);
		usmode(curbp,MDINST);
		stmode(curbp,MDVISL);
	} else if (mode == MDINST) {
		usmode(curbp,MDVISL);
		usmode(curbp,MDLOCK);
		stmode(curbp,MDINST);
	} else {
		stmode(curbp,mode);
	}
}

/* macro compatible change mode functions */
int set_lock_mode(int f, int n)
{
	set_mode_for_buffer(MDLOCK);
	return true;
}

int set_visual_mode(int f, int n)
{
	set_mode_for_buffer(MDVISL);
	return true;
}

int set_insert_mode(int f, int n)
{
	set_mode_for_buffer(MDINST);
	return true;
}

int set_command_mode(int f, int n)
{
	set_prompt_mask(COMMAND_MODE_MASK);
	set_mode_for_buffer(MDCMMD);
}

/*
 *	set current buffer to given bf and update cursor position
 */
void change_current_buffer(buffer *bf)
{
	curbp = bf;
	cursor_row = curbp->mtop + (curbp->clindex - curbp->loffset);
	if (current_line == NULL) {
		cursor_col = curbp->mleft;
	} else
		update_position();
}

/*
 *	set next buffer in window for active window
 */
int next_buffer_in_window(int f,int n)
{
	buffer *bf = L_LINK_NEXT(curbp);
	if (bf == NULL) {
		 showmsg(true,"Last buffer");
		 return false;
	}
	curwp->cbindex++;
	change_current_buffer(bf);
	return true;
}

int prev_buffer_in_window(int f,int c)
{
	buffer *bf = L_LINK_PREV(curbp);
	if (bf == NULL) {
		showmsg(true,"First buffer");
		return false;
	}
	curwp->cbindex--;
	change_current_buffer(bf);
	return true;
}

/*
 *	becareful when use this function , it will make free all 
 *	properties of current buffer , and curbp too , so dont 
 *	set curbp here
 */
void free_buffer()
{
	for (line *ln = curbp->lline;ln != NULL;ln = L_LINK_PREV(ln)) {
		free(ln->chars);
	}
	free(curbp);
}

/*
 *	this routhien will remove buffer from window
 *	also it will append its space to next or prev buffer
 *	return alone buffer , if it was last buffer in window
 */
int remove_buffer()
{
	buffer *new_one = L_LINK_PREV(curbp);
	if (new_one == NULL) {
    	new_one = L_LINK_NEXT(curbp);
    	if (new_one == NULL) {
        	free_buffer();
        	return ALONEBUFFER;
    	}
	}
	if (new_one == L_LINK_NEXT(curbp)) {
		new_one->mtop = curbp->mtop;
	}
	L_LINK_REMOVE(curbp);
	if (curbp == curwp->fbuffer)
		curwp->fbuffer = new_one;
	new_one->nrow += curbp->nrow;
	curwp->bcount--;
	free_buffer();
	change_current_buffer(new_one);
	curbp->flags |= FREDRW;
	return true;
}

/*
 *	return buffer by index 
 *	we have to set curwp to our intended window 
 *	becuase it search in curwp
 */
buffer *get_buffer_by_index(int index) 
{
	int i = 0;
	for (buffer *bf = curwp->fbuffer;bf != NULL;bf = L_LINK_NEXT(bf),i++) {
		if (i == index)
			return bf;
	}
	return NULL;
}
