#ifndef BASICS_H
# define BASICS_H

#define print(str)		printf("%s", (str))
#define printl(str)		printf("%s\n", (str))

#ifndef EOL
# define EOL '\n'
#endif

#ifndef var
#define var	extern
#endif

typedef unsigned char	    uchar;
typedef unsigned short	    ushort;
typedef unsigned int	    uint;
typedef unsigned long       ulong;
typedef signed char         int8;
typedef unsigned char       uint8;
typedef signed short int    int16;
typedef unsigned short int  uint16;
typedef signed int          int32;
typedef unsigned int        uint32;
typedef signed long int     int64;
typedef unsigned long int   uint64;
typedef char * 				string;

/*
 * Boolean constants
 */
#ifndef TRUE
# define FALSE	0	    // note: this is an int, not a long!
# define TRUE	1
#endif

/*
 * Some usefull constants for strings :D
 */
#ifndef IN
# define OUT 0
# define IN 1
#endif

/*
 * Min and Max :D
 */
#ifndef MAX
# define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif
#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/*
 * Two string Concatination
 */
#ifndef CONCAT
# define CONCAT(a,b) a ## b
#endif

/* 
 * n string concatination
 */
#define strcpyall(buf, offset, ...) do{ \
    char *bp=(char*)(buf+offset); /*so we can add to the end of a string*/ \
    const char *s, \
    *a[] = { __VA_ARGS__,NULL}, \
    **ss=a; \
    while((s=*ss++)) \
    while((*s)&&(++offset<(int)sizeof(buf))) \
    *bp++=*s++; \
    if (offset!=sizeof(buf))*bp=0; \
}while(0)

/* 
 * foreach in c haha
 */
#define foreach(item, array) \
    for(int keep=1, \
            count=0,\
            size=sizeof (array)/sizeof *(array); \
            keep && count != size; \
            keep = !keep, count++) \
for(item = (array)+count; keep; keep = !keep)

/*
 * LENGTH of an array
 */
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof (array[0]))

/*
 * is even and odd for number
 */
#define IS_EVEN( num )  (!IS_ODD( (num) ))
#define IS_ODD( num )   ((num) & 1)

/*
 *  Is ctrl key
 */
#define CTRL_KEY(k) ((k) & 0x1f)

#endif


/*
 *  this macro is from : https://github.com/postgres/postgres/blob/73aa5e0cafd0d577fe464ed1d9ac317103f27ea4/src/include/utils/elog.h#L313
 */
/*----------
 * API for catching ereport(ERROR) exits.  Use these macros like so:
 *
 *		PG_TRY();
 *		{
 *			... code that might throw ereport(ERROR) ...
 *		}
 *		PG_CATCH();
 *		{
 *			... error recovery code ...
 *		}
 *		PG_END_TRY();
 *
 * (The braces are not actually necessary, but are recommended so that
 * pgindent will indent the construct nicely.)  The error recovery code
 * can either do PG_RE_THROW to propagate the error outwards, or do a
 * (sub)transaction abort. Failure to do so may leave the system in an
 * inconsistent state for further processing.
 *
 * For the common case that the error recovery code and the cleanup in the
 * normal code path are identical, the following can be used instead:
 *
 *		PG_TRY();
 *		{
 *			... code that might throw ereport(ERROR) ...
 *		}
 *		PG_FINALLY();
 *		{
 *			... cleanup code ...
 *		}
 *      PG_END_TRY();
 *
 * The cleanup code will be run in either case, and any error will be rethrown
 * afterwards.
 *
 * You cannot use both PG_CATCH() and PG_FINALLY() in the same
 * PG_TRY()/PG_END_TRY() block.
 *
 * Note: while the system will correctly propagate any new ereport(ERROR)
 * occurring in the recovery section, there is a small limit on the number
 * of levels this will work for.  It's best to keep the error recovery
 * section simple enough that it can't generate any new errors, at least
 * not before popping the error stack.
 *
 * Note: an ereport(FATAL) will not be caught by this construct; control will
 * exit straight through proc_exit().  Therefore, do NOT put any cleanup
 * of non-process-local resources into the error recovery section, at least
 * not without taking thought for what will happen during ereport(FATAL).
 * The PG_ENSURE_ERROR_CLEANUP macros provided by storage/ipc.h may be
 * helpful in such cases.
 *
 * Note: if a local variable of the function containing PG_TRY is modified
 * in the PG_TRY section and used in the PG_CATCH section, that variable
 * must be declared "volatile" for POSIX compliance.  This is not mere
 * pedantry; we have seen bugs from compilers improperly optimizing code
 * away when such a variable was not marked.  Beware that gcc's -Wclobbered
 * warnings are just about entirely useless for catching such oversights.
 *----------
 */
//#define PG_TRY()  \
//	do { \
//		sigjmp_buf *_save_exception_stack = PG_exception_stack; \
//		ErrorContextCallback *_save_context_stack = error_context_stack; \
//		sigjmp_buf _local_sigjmp_buf; \
//		bool _do_rethrow = false; \
//		if (sigsetjmp(_local_sigjmp_buf, 0) == 0) \
//		{ \
//			PG_exception_stack = &_local_sigjmp_buf
//
//#define PG_CATCH()	\
//		} \
//		else \
//		{ \
//			PG_exception_stack = _save_exception_stack; \
//			error_context_stack = _save_context_stack
//
//#define PG_FINALLY() \
//		} \
//		else \
//		_do_rethrow = true; \
//		{ \
//			PG_exception_stack = _save_exception_stack; \
//			error_context_stack = _save_context_stack
//
//#define PG_END_TRY()  \
//		} \
//		if (_do_rethrow) \
//		PG_RE_THROW(); \
//		PG_exception_stack = _save_exception_stack; \
//		error_context_stack = _save_context_stack; \
//	} while (0)
