dist_noinst_DATA = \
	openinput.texi \
	version.texi \
	version.texi.in \
	top.texi \
	intro.texi \
	sample.texi \
	design.texi \
	fdl.texi \
	general_flowchart.dia

MANUALDIR=../manual

# Simple directory target
.PHONY: manual-dir
manual-dir:
	mkdir -p $(MANUALDIR)

# Non-phony texi2html target
if ENABLE_TEXI2HTML
TEXTARG=$(MANUALDIR)/index.html
$(TEXTARG): *texi
	texi2html --Verbose --init-file=xhtml.init \
		  --output=$(MANUALDIR) --sec-nav \
		  --number --toc-links --top-file=index.html \
		  --menu --split=chapter openinput.texi
else
TEXTARG=
endif

# Non-phony dia target
if ENABLE_DIA
$(MANUALDIR)/%.png: %.dia
	@DIA_PATH@ --export=$(MANUALDIR)/$@ --filter=png $< 
DIATARG=$(patsubst %.dia,$(MANUALDIR)/%.png,$(wildcard *.dia))
else
DIATARG=
endif

info-local: manual-dir $(DIATARG) $(TEXTARG)

all-local: info-local


install-data-local:
	mkdir -p ${docdir}/manual
	cp -r $(MANUALDIR) ${docdir}


clean-local:
	rm -rf $(MANUALDIR)

uninstall-local:
	rm -rf ${docdir}/manual
	rmdir --ignore-fail-on-non-empty ${docdir}
