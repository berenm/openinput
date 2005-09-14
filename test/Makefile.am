TESTS = @TEST_PROGS@

EXTRA_PROGRAMS = \
keynametest \
footest \
x11test \
x11actiontest

noinst_PROGRAMS = @TEST_PROGS@

footest_SOURCES = \
footest.c

x11test_SOURCES = \
x11test.c

x11actiontest_SOURCES = \
x11actiontest.c

keynametest_SOURCES = \
keynametest.c

LDADD = \
$(top_srcdir)/src/libopeninput.la

INCLUDES = \
-I$(top_srcdir)/include
