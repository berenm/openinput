dnl Autoconf
AC_PREREQ(2.59)
AC_INIT([libsinp], 0.0.1, [makob@makob.dk])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

dnl Checks for programs.
CFLAGS="-g -O0 -Wall"
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

dnl Debugging mode
AC_ARG_ENABLE(debug,
   AS_HELP_STRING([--enable-debug], [enable extra debugging in the binary (default=no)]),
   [], enable_debug=no)

dnl Platforms
AC_ARG_ENABLE(foo,
   AS_HELP_STRING([--enable-foo], [enable debugging input system (default=yes)]),
   [], enble_foo=yes)
AC_ARG_ENABLE(x11,
   AS_HELP_STRING([--enable-x11], [enable X11 window system input (default=yes)]),
   [], enable_x11=yes)

dnl Check for libraries and header files
AC_CHECK_LIB(pthread, main)
AC_PATH_X
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h locale.h malloc.h stddef.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])

dnl Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_REPLACE_FNMATCH
AC_HEADER_MAJOR
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([getcwd gettimeofday isascii memchr memmove memset rmdir select strchr strcspn strdup strerror strpbrk strrchr strspn strstr])

dnl Setup defines
if test "x$enable_debug" = xyes; then
   AC_DEFINE([DEBUG], [1], [Internal debugging]) 
fi

if test "x$enable_foo" = xyes; then
   AC_DEFINE([ENABLE_FOO], [1], [Debug input system])
   PLATDIRS="$PLATDIRS foo"
   PLATLIBS="$PLATLIBS foo/libfoo.la"
fi

if test "x$enable_x11" = xyes; then
   AC_DEFINE([ENABLE_X11], [1], [X11 window system])
   PLATDIRS="$PLATDIRS x11"
   PLATLIBS="$PLATLIBS x11/libx11.la"
fi

AC_SUBST(PLATDIRS)
AC_SUBST(PLATLIBS)

dnl Makefiles to be generated
AC_CONFIG_FILES([
	sinp.pc \
	Makefile \
	include/Makefile \
	test/Makefile \
	src/Makefile \
	src/x11/Makefile \
	src/foo/Makefile])

dnl Finish
AC_OUTPUT
