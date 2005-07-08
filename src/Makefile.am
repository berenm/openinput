SUBDIRS = \
foo \
x11

lib_LTLIBRARIES = \
libsinp.la

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
-I$(top_srcdir)/include

PLATFORMS =
PLATLIBS = 

# Find dirs and libs of platforms
if ENABLE_FOO
	PLATFORMS += foo
	PLATLIBS += foo/libfoo.la
endif
if ENABLE_X11
	PLATFORMS += x11
	PLATLIBS += x11/libx11.la
endif

libsinp_la_DEPENDENCIES = $(PLATLIBS)
libsinp_la_LIBADD = $(PLATLIBS)
