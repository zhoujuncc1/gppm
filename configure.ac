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
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])

BOOST_REQUIRE([1.60])

AC_CHECK_LIB([m],[cos])
AC_CHECK_LIB([gslcblas],[cblas_dgemm])
AC_CHECK_LIB([gsl],[gsl_blas_dgemm])

AC_CONFIG_FILES(Makefile src/Makefile test/Makefile examples/Makefile)
AC_CONFIG_SRCDIR(src)
AC_OUTPUT


