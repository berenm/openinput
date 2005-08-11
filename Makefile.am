SUBDIRS = src include test

include scripts/doxygen.am

DIST_EXTRA = TODO $(DX_CONFIG)

pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = openinput.pc

dist-hook:
	mkdir ${distdir}/doc
	cp ${srcdir}/doc/*txt ${distdir}/doc
	cp -r ${srcdir}/doc/doxygen ${distdir}/doc/
