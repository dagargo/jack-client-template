#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([jclient],[0.2],[dagargo@gmail.com])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
LT_INIT
AC_SEARCH_LIBS([sqrt], [m])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

PKG_CHECK_MODULES(JACK, jack >= 0.100.0)

# Checks for header files.
AC_CHECK_HEADERS([stdio.h unistd.h stdlib.h errno.h string.h signal.h limits.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
