AM_CPPFLAGS = @VMOD_INCLUDES@ -Wall -Werror

vmoddir = @VMOD_DIR@
vmod_LTLIBRARIES = libvmod_dynamo.la

libvmod_dynamo_la_LDFLAGS = -module -export-dynamic -avoid-version -shared

libvmod_dynamo_la_SOURCES = \
	vmod_dynamo.c

nodist_libvmod_dynamo_la_SOURCES = \
	vcc_if.c \
	vcc_if.h

vmod_dynamo.lo: vcc_if.c vcc_if.h

vcc_if.c: vcc_if.h

vcc_if.h: @VMODTOOL@ $(top_srcdir)/src/vmod_dynamo.vcc
	@VMODTOOL@ $(top_srcdir)/src/vmod_dynamo.vcc

VMOD_TESTS = $(top_srcdir)/src/tests/*.vtc
.PHONY: $(VMOD_TESTS)

$(top_srcdir)/src/tests/*.vtc: libvmod_dynamo.la
	@VARNISHTEST@ -Dvarnishd=@VARNISHD@ -Dvmod_topbuild=$(abs_top_builddir) $@

check: $(VMOD_TESTS)

EXTRA_DIST = \
	vmod_dynamo.vcc \
	$(VMOD_TESTS)

CLEANFILES = \
	$(builddir)/vcc_if.c \
	$(builddir)/vcc_if.h \
	$(builddir)/vmod_dynamo.rst \
	$(builddir)/vmod_dynamo.man.rst
