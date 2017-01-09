#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(Arm Examples, 1.0, Guillaume.Huard@imag.fr, arm-examples)
AC_CONFIG_AUX_DIR([../build-aux])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([file1.c])
AC_CONFIG_HEADER([config.h])

AC_MSG_NOTICE([configuring ARM examples])

# Checks for programs.
_save_LDFLAGS="$LDFLAGS"
LDFLAGS="-nostdlib -Wl,--entry=0"
AC_PROG_CC
LDFLAGS="$_save_LDFLAGS"
AS_CASE(["$CC"],
  [*-none-eabi-*], [
    AM_LDFLAGS="-nostdlib -Wl,--entry=0"
    _have_no_stdlib=1
  ],
  [ _have_no_stdlib=0 ] )
AM_CONDITIONAL([HAVE_NO_STDLIB], [test "$_have_no_stdlib" = 1])
AX_CHECK_COMPILE_FLAG_ARCHIVE([-no-pie], [AM_CFLAGS="-no-pie"], [], [-Werror])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])
AM_PROG_AS
AM_PROG_CC_C_O

AC_MSG_CHECKING([whether the compiler is targeting ARM])
if $CC -dumpmachine | grep arm > /dev/null ; then
  AC_MSG_RESULT([yes])
  have_arm_compiler=yes
else
  AC_MSG_RESULT([no])
  have_arm_compiler=no
  AC_MSG_WARN([An ARM cross-compiler is required to compile the examples])
fi
AM_CONDITIONAL([HAVE_ARM_COMPILER],[test $have_arm_compiler = yes])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
