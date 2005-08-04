DIST_SUBDIRS = \
foo \
x11 \
unixsignal

SUBDIRS = @PLATDIRS@

lib_LTLIBRARIES = \
libopeninput.la

libopeninput_la_DEPENDENCIES = @PLATLIBS@
libopeninput_la_LIBADD = @PLATLIBS@

libopeninput_la_SOURCES = \
bootstrap.h \
internal.h \
main.c \
queue.c \
debug.c \
device.c \
events.c \
appstate.c \
mouse.c \
keyboard.c \
keynames.c \
action.c

INCLUDES = \
-I$(top_srcdir) \
-I$(top_srcdir)/include
