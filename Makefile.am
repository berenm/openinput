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
	cp -r ${srcdir}/nongnu ${distdir}
	find ${distdir}/nongnu -name *svn -type d|xargs -l1 rm -rf
