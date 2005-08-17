SUBDIRS = src include test

include scripts/doxygen.am

DIST_EXTRA = TODO $(DX_CONFIG)

pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = openinput.pc

dist-hook:
	mkdir ${distdir}/doc
	cp ${srcdir}/doc/*txt ${distdir}/doc
	mkdir ${distdir}/doc/doxygen
	cp ${srcdir}/doc/doxygen/*dox ${distdir}/doc/doxygen/
	cp ${srcdir}/doc/doxygen/*html ${distdir}/doc/doxygen/
	cp ${srcdir}/doc/doxygen/*css ${distdir}/doc/doxygen/
	cp ${srcdir}/doc/doxygen/*png ${distdir}/doc/doxygen/
	cp ${srcdir}/doc/doxygen/Doxyfile ${distdir}/doc/doxygen/
