# Test programs
TESTS = \
	@TEST_PROGS@

EXTRA_PROGRAMS = \
	keynametest \
	footest \
	x11test \
	x11actiontest \
	openclose \
	win32test

noinst_PROGRAMS = \
	@TEST_PROGS@

LDADD = \
	$(top_srcdir)/src/libopeninput.la

INCLUDES = \
	-I$(top_srcdir)/include

# Foo driver
footest_SOURCES = \
	footest.c

# X11 driver
x11test_SOURCES = \
	x11test.c \
	platform.c

# X11 driver with action mapping
x11actiontest_SOURCES = \
	x11actiontest.c

# Mouse and keyboard names
keynametest_SOURCES = \
	keynametest.c

# Win32 driver
win32test_SOURCES = \
	win32test.c \
	platform.c

win32test_LDADD = \
	-luser32 -lgdi32 \
        $(top_srcdir)/src/libopeninput.la