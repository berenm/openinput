# The OpenInput library
DIST_SUBDIRS = \
	foo \
	x11 \
	unixsignal \
	linuxjoy \
	win32 \
	dx9

SUBDIRS = \
	@BUILD_DIRS@

lib_LTLIBRARIES = \
	libopeninput.la

libopeninput_la_DEPENDENCIES = \
	@BUILD_LIBS@

libopeninput_la_LIBADD = \
	@BUILD_LIBS@ \
	@SYSTEM_LIBS@

libopeninput_la_LDFLAGS = \
	-no-undefined \
	-release @LT_RELEASE@ \
	-version-info @LT_CURRENT@:@LT_REVISION@:@LT_AGE@

libopeninput_la_SOURCES = \
	bootstrap.h \
	internal.h \
	private.h \
	main.c \
	queue.c \
	debug.c \
	device.c \
	events.c \
	appstate.c \
	mouse.c \
	keyboard.c \
	keynames.c \
	action.c \
	joystick.c

INCLUDES = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include
