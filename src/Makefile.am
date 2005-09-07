DIST_SUBDIRS = \
	foo \
	x11 \
	unixsignal \
	linuxjoy

SUBDIRS = \
	@PLATDIRS@

lib_LTLIBRARIES = \
	libopeninput.la

libopeninput_la_DEPENDENCIES = \
	@PLATLIBS@

libopeninput_la_LIBADD = \
	 @PLATLIBS@

libopeninput_la_LDFLAGS = \
	-noundefined \
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
