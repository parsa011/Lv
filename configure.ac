#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([lv], [0.0.1], [parsa32569@gmail.com])
AC_CONFIG_SRCDIR([src/lv.c])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
#AC_CHECK_LIB([gtk-x11-2.0], [gtk_init], [], [
#			  echo "The gtk library is required for this program."
#			  exit -1])
#AC_CHECK_LIB([cairo], [cairo_set_font_size], [], [
#			  echo "The cairo library is required for this program."
#			  exit -1])
#AC_CHECK_LIB([fontconfig], [FcInitLoadConfig], [], [
#			  echo "The fontconfig library is required for this program."
#			  exit -1])
#AC_CHECK_LIB([freetype], [FT_Init_FreeType], [], [
#			  echo "The freetype library is required for this program."
#			  exit -1])

# Checks for header files.
# Checks for header files.
AC_SYS_LARGEFILE
AC_HEADER_DIRENT
AC_SYS_POSIX_TERMIOS
if test "$ac_cv_sys_posix_termios" = "yes"; then
	AC_DEFINE(HAVE_POSIX_TERMIOS, 1, [We have <termios.h> and <sys/termios.h>])
	else
		AC_CHECK_HEADERS([termio.h sys/termio.h])
		if test "$ac_cv_header_termio_h" = "yes" -a "$ac_cvs_header_sys_termio_h" = "yes" ]; then
			AC_DEFINE(HAVE_SYSV_TERMIO, 1, [We have <termio.h> and <sys/termio.h>])
			fi
fi
AC_CHECK_HEADERS([errno.h fcntl.h locale.h sgtty.h stdlib.h string.h])
AC_CHECK_HEADERS([sys/ioctl.h sys/param.h sys/time.h unistd.h utime.h])
AC_CHECK_HEADERS([sys/dirent.h time.h pwd.h paths.h pty.h libutil.h])
AC_CHECK_HEADERS([sys/types.h sys/stat.h sys/wait.h limits.h signal.h])
AC_CHECK_HEADERS([curses.h term.h utmp.h sys/utime.h])

AC_CHECK_HEADERS([selinux/selinux.h selinux/context.h])
if test "$ac_cv_header_selinux_selinux_h" = "yes" -a "$ac_cv_header_selinux_context_h" = "yes" ; then
	echo "Enabling SELinux context copying!"
	AC_DEFINE(HAVE_SELINUX, 1, [We have SELinux headers])
fi

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile
	 src/Makefile])

# Added after the first 'autoconf' invocation and 'aclocal'
AM_INIT_AUTOMAKE([gnu subdir-objects])
AUTOMAKE_OPTIONS = subdir-objects

AC_OUTPUT
