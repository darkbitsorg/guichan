lib_LTLIBRARIES = libguichan_opengl.la

AM_CPPFLAGS = -I$(top_srcdir)/include

libguichan_opengl_la_LIBADD  = $(top_builddir)/src/libguichan.la -lGL
libguichan_opengl_la_LDFLAGS = -no-undefined -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libguichan_opengl_la_SOURCES =	\
	opengl.cpp		\
	openglimage.cpp		\
	openglgraphics.cpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = guichan_opengl.pc

EXTRA_DIST = guichan_opengl.pc.in
