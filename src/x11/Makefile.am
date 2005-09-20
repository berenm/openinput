# X11 window system device
noinst_LTLIBRARIES = \
	libx11.la

libx11_la_SOURCES = \
	x11.c \
	x11.h \
	x11_events.c \
	x11_translate.c

INCLUDES = \
	$(X_CFLAGS) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src
