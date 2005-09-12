dnl Prerequisites
AC_PREREQ(2.59)

dnl Library version
define(LIBOI_MAJOR_VERSION, 0) # Bump on very huge changes, zero rest
define(LIBOI_MINOR_VERSION, 1) # Bump on big changes, zero remaining
define(LIBOI_PATCH_VERSION, 0) # Bump on changes
define(LIBOI_INFACE_AGE, 0)    # Bump on changes, zero on binary breakage
define(LIBOI_BINARY_AGE, 0)    # Bump on changes, zero on binary breakage
define(LIBOI_VERSION, OI_MAJOR_VERSION.OI_MINOR_VERSION.OI_PATCH_VERSION)

dnl Initialize autoconf
AC_INIT([OpenInput], LIBOI_VERSION, [makob@makob.dk], [openinput])
AC_CANONICAL_TARGET

dnl Check for programs.
CFLAGS="-g -O0 -Wall -Werror"
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl Versioning
OI_MAJOR_VERSION=LIBOI_MAJOR_VERSION
OI_MINOR_VERSION=LIBOI_MINOR_VERSION
OI_PATCH_VERSION=LIBOI_PATCH_VERSION
OI_INFACE_VERSION=LIBOI_INFACE_AGE
OI_BINARY_VERSION=LIBOI_BINARY_AGE
OI_VERSION=LIBOI_MAJOR_VERSION.LIBOI_MINOR_VERSION.LIBOI_PATCH_VERSION
LT_RELEASE=$OI_MAJOR_VERSION.$OI_MINOR_VERSION
LT_CURRENT=`expr $OI_PATCH_VERSION - $OI_INFACE_VERSION`
LT_REVISION=$OI_BINARY_VERSION
LT_AGE=`expr $OI_BINARY_VERSION - $OI_INFACE_VERSION`
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)
AC_SUBST(OI_MAJOR_VERSION)
AC_SUBST(OI_MINOR_VERSION)
AC_SUBST(OI_PATCH_VERSION)
AC_SUBST(OI_VERSION)

dnl Initialize automake
AM_INIT_AUTOMAKE([OpenInput], $OI_VERSION)
AC_CONFIG_HEADERS([config.h])
ARCHDET

dnl Doxygen
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(OpenInput, [doc/doxygen/Doxyfile], [doc/api/])

dnl Default test programs
TESTPRGS="keynametest"

dnl Debugging mode
AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug], [enable extra debugging in the binary (default=no)]),
    [], enable_debug=no)
if test x$enable_debug = xyes; then
    AC_DEFINE([DEBUG], [1], [Internal debugging]) 
fi

dnl Driver "foo"
AC_ARG_ENABLE(foo,
    AS_HELP_STRING([--enable-foo], [enable the foo debug input system (default=no)]),
    [], enble_foo=no)
if test x$enable_foo = xyes; then
    AC_DEFINE([ENABLE_FOO], [1], [Debug input system])
    PLATDIRS="$PLATDIRS foo"
    PLATLIBS="$PLATLIBS foo/libfoo.la"
    TESTPRGS="$TESTPRGS footest"
fi

dnl Driver "x11"
AC_ARG_ENABLE(x11,
    AS_HELP_STRING([--enable-x11], [enable X11 window system input (default=yes)]),
    [], enable_x11=yes)
if test x$enable_x11 = xyes; then
    AC_PATH_X
    AC_PATH_XTRA
    if test x$have_x = xyes; then
        AC_DEFINE([ENABLE_X11], [1], [X11 window system])
        PLATDIRS="$PLATDIRS x11"
        PLATLIBS="$PLATLIBS x11/libx11.la"
        TESTPRGS="$TESTPRGS x11test x11actiontest"
    fi
fi

dnl Driver "unixsignal"
AC_ARG_ENABLE(unixsignal,
    AS_HELP_STRING([--enable-unixsignal], [enable UNIX signal handler (default=yes)]),
    [], enable_unixsignal=yes)
if test x$enable_unixsignal = xyes; then
    have_unixsignal=no
    AC_CHECK_HEADER(signal.h, have_unixsignal=yes)
    if test x$have_unixsignal = xyes; then
       AC_DEFINE([ENABLE_UNIXSIGNAL], [1], [UNIX signal handler])
       PLATDIRS="$PLATDIRS unixsignal"
       PLATLIBS="$PLATLIBS unixsignal/libunixsignal.la"
    fi
fi

dnl Driver "linuxjoy"
AC_ARG_ENABLE(linuxjoy,
    AS_HELP_STRING([--enable-linuxjoy], [enable GNU/Linux joystick driver (default=yes)]),
    [], enable_linuxjoy=yes)
if test x$enable_linuxjoy = xyes; then
    have_linuxjoy=no
    AC_CHECK_HEADER([linux/joystick.h],[have_linuxjoy=yes])
    if test x$have_linuxjoy = xyes; then
        AC_DEFINE([ENABLE_LINUXJOY], [1], [GNU/Linux joystick driver])
        PLATDIRS="$PLATDIRS linuxjoy"
        PLATLIBS="$PLATLIBS linuxjoy/liblinuxjoy.la"
    fi
fi

dnl Driver "win32"
AC_ARG_ENABLE(win32,
     AS_HELP_STRING([--enable-win32], [enable native Microsoft Windows input driver (default=yes)]),
     [], enable_win32=yes)
if test x$enable_win32 = xyes; then
     AC_CHECK_HEADER([windows.h], [have_win32=yes])
     if test x$have_win32 = xyes; then
         AC_DEFINE([ENABLE_WIN32], [1], [Microsoft Windows input driver])
         PLATDIRS="$PLATDIRS win32"
         PLATLIBS="$PLATLIBS win32/libwin32.la"
     fi
fi

dnl Check for libraries and header files
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
AC_HEADER_MAJOR
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([gettimeofday isascii memchr memset select strchr strcspn strdup strrchr strspn strstr nanosleep])

dnl Make variables available
AC_SUBST(TESTPRGS)
AC_SUBST(PLATDIRS)
AC_SUBST(PLATLIBS)

dnl Files to be processed
AC_CONFIG_FILES([
	openinput.pc \
	doc/doxygen/Doxyfile \
	Makefile \
	include/Makefile \
	test/Makefile \
	src/Makefile \
	src/foo/Makefile \
	src/x11/Makefile \
	src/unixsignal/Makefile \
	src/linuxjoy/Makefile \
	src/win32/Makefile])

dnl Special file containinig auto-filled version
AC_CONFIG_FILES([include/openinput_h_version], \
	[mv include/openinput_h_version include/openinput.h])

dnl Finish
AC_OUTPUT
