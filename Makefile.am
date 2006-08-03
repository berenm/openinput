SUBDIRS = \
	src \
	include \
	test \
	doc

dist_doc_DATA = \
	AUTHORS \
	COPYING \
	README \
	INSTALL \
	ChangeLog

pkgconfigdir = $(libdir)/pkgconfig

dist_pkgconfig_DATA = openinput.pc

dist-hook:
	mkdir -p ${distdir}/nongnu
	cp ${srcdir}/nongnu/* ${distdir}/nongnu/
