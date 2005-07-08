DIST_SUBDIRS = \
foo \
x11

SUBDIRS = @PLATDIRS@

lib_LTLIBRARIES = \
libsinp.la

libsinp_la_DEPENDENCIES = @PLATLIBS@
libsinp_la_LIBADD = @PLATLIBS@

libsinp_la_SOURCES = \
device.h \
bootstrap.h \
internal.h \
main.c \
queue.c \
debug.c \
device.c \
events.c

INCLUDES = \
-I$(top_srcdir) \
-I$(top_srcdir)/include
