lib_LTLIBRARIES = libguichan_allegro.la

AM_CPPFLAGS = -I$(top_srcdir)/include

libguichan_allegro_la_LIBADD = $(top_builddir)/src/libguichan.la -lalleg
libguichan_allegro_la_LDFLAGS = -no-undefined -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libguichan_allegro_la_SOURCES =		\
		allegro.cpp		\
		allegrofont.cpp		\
		allegrographics.cpp	\
		allegroimage.cpp	\
		allegroimageloader.cpp	\
		allegroinput.cpp
