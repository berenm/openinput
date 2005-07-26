EXTRA_PROGRAMS = \
footest \
x11test

noinst_PROGRAMS = @TESTPRGS@

footest_SOURCES = \
footest.c \
footest.h

footest_LDADD = \
$(top_srcdir)/src/libsinp.la

x11test_SOURCES = \
x11test.c

x11test_LDADD = \
$(top_srcdir)/src/libsinp.la

# Extra
INCLUDES = \
-I$(top_srcdir)/include
