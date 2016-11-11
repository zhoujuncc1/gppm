dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(gppm, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

AM_PROG_AR
LT_INIT

AC_PROG_CXX
AC_PROG_LEX
AC_PROG_YACC

AC_CONFIG_MACRO_DIRS([m4])

AC_SEARCH_LIBS([boost])
AC_CONFIG_FILES(Makefile src/Makefile test/Makefile)
AC_CONFIG_SRCDIR(src)
AC_OUTPUT

