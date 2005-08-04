SUBDIRS = src include test

DIST_EXTRA = TODO

pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = openinput.pc

dist-hook:
	mkdir ${distdir}/doc
	cp ${srcdir}/doc/*txt ${distdir}/doc
