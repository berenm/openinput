SUBDIRS = src include test

include scripts/doxygen.am

DIST_EXTRA = TODO $(DX_CONFIG)

pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = openinput.pc

dist-hook:
	mkdir ${distdir}/doc
	cp ${srcdir}/doc/*txt ${distdir}/doc
	mkdir ${distdit}/doc/html
	cp -r ${srcdir}/doc/api/html/* ${distdit}/doc/api/html
