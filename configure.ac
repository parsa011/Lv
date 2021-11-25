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
AC_CHECK_HEADERS([stdlib.h string.h stdbool.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile
	 src/Makefile])

# Added after the first 'autoconf' invocation and 'aclocal'
AM_INIT_AUTOMAKE([gnu subdir-objects])
AUTOMAKE_OPTIONS = subdir-objects

AC_OUTPUT
