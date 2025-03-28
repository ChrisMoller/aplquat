
lib_LTLIBRARIES = libaplquat.la

libaplquat_la_SOURCES = aplquat.cc Quat.cc  Quat.hh
libaplquat_la_CPPFLAGS = -I$(APL_SOURCES) -I$(APL_SOURCES)/src

noinst_LTLIBRARIES =


.FORCE:

gitversion.h : .FORCE
	git log  -1 --pretty='format:#define GIT_VERSION "%h %ci"' > xxx
	echo "" >> xxx
	cmp -s xxx $@; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then rm xxx; else mv xxx $@; fi
