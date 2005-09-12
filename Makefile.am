SUBDIRS = src include test

include scripts/doxygen.am

pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = openinput.pc

dist-hook:
	mkdir -p ${distdir}/doc/doxygen
	cp ${srcdir}/doc/*txt ${distdir}/doc
	cp ${srcdir}/doc/doxygen/*dox ${distdir}/doc/doxygen/
	cp ${srcdir}/doc/doxygen/*html ${distdir}/doc/doxygen/
	cp ${srcdir}/doc/doxygen/*css ${distdir}/doc/doxygen/
	cp ${srcdir}/doc/doxygen/*png ${distdir}/doc/doxygen/
	cp ${srcdir}/doc/doxygen/Doxyfile ${distdir}/doc/doxygen/
