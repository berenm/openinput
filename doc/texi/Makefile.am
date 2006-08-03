dist_noinst_DATA = \
	openinput.texi \
	version.texi \
	version.texi.in \
	top.texi \
	intro.texi \
	sample.texi

# Non-phony texi2html target
TEXTARG=../manual/index.html
$(TEXTARG): version.texi
	mkdir -p ../manual
	texi2html --Verbose --init-file=xhtml.init \
		  --output=../manual --sec-nav \
		  --number --toc-links --top-file=index.html \
		  --menu --split=chapter openinput.texi

if ENABLE_TEXI2HTML
info-local: $(TEXTARG)

all-local: info-local

install-data-local:
	mkdir -p ${docdir}/manual
	cp -r ../manual ${docdir}
endif

clean-local:
	rm -rf ../manual

uninstall-local:
	rm -rf ${docdir}/manual
	rmdir --ignore-fail-on-non-empty ${docdir}
