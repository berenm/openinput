dist_noinst_DATA = \
	Doxyfile.in \
	banner.png \
	footer.html \
	header.html \
	style.css \
	intgroups.dox \
	mainpage.dox \
	pubgroups.dox

# Non-phony Doxygen target
DOXTARG=../api/openinput.tag
$(DOXTARG): Doxyfile
	mkdir -p ../api/html
	doxygen Doxyfile

if ENABLE_DOXYGEN
info-local: $(DOXTARG)

all-local: info-local

install-data-local:
	mkdir -p ${docdir}/api
	cp -r ../api ${docdir}
endif

clean-local:
	rm -rf ../api


uninstall-local:
	rm -rf ${docdir}/api
	rmdir --ignore-fail-on-non-empty ${docdir}
