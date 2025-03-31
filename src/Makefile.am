AM_MAKEFLAGS = -j $(MAKE_J)

AM_CXXFLAGS = $(CXX_RDYNAMIC) -I $(srcdir)/.. -g -O2

common_LDFLAGS = -avoid-version
common_LDFLAGS += -export-dynamic

lib_LTLIBRARIES = libaplquat.la

libaplquat_la_SOURCES = aplquat.cc Quat.cc  Quat.hh kwds.h
libaplquat_la_CPPFLAGS = -I$(APL_SOURCES) -I$(APL_SOURCES)/src
libaplquat_la_CXXFLAGS = $(AM_CXXFLAGS)
libaplquat_la_LDFLAGS = $(common_LDFLAGS)

BUILT_SOURCES = kwds.h
CLEANFILES = kwds.h

noinst_LTLIBRARIES =

EXTRA_DIST = kwds.m4


kwds.h: kwds.m4
	m4 $^ >$@

.FORCE:

gitversion.h : .FORCE
	git log  -1 --pretty='format:#define GIT_VERSION "%h %ci"' > xxx
	echo "" >> xxx
	cmp -s xxx $@; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then rm xxx; else mv xxx $@; fi
