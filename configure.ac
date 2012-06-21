#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([Resonance], [1.0], [Daniel Stepp <willstepp@gmail.com>], [resonance], [http://resonance-asm.com/])
AC_CONFIG_SRCDIR([src/AeroButton.h])
AM_INIT_AUTOMAKE()
#AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
                 Makefile
                 src/Makefile
                 ])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES(LIBQTCORE, QtCore)
PKG_CHECK_MODULES(LIBQTGUI, QtGui)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([mkdir rmdir])

AC_OUTPUT
