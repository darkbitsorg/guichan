lib_LTLIBRARIES = libguichan_sdl2.la

AM_CPPFLAGS = -I$(top_srcdir)/include $(SDL2_CFLAGS)

libguichan_sdl2_la_LIBADD  = $(top_builddir)/src/libguichan.la $(SDL2_LIBS)
libguichan_sdl2_la_LDFLAGS = -no-undefined -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libguichan_sdl2_la_SOURCES =	\
	sdl2.cpp		\
	sdl2graphics.cpp	\
	sdl2image.cpp		\
	sdl2imageloader.cpp	\
	sdl2input.cpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = guichan_sdl2.pc

EXTRA_DIST = guichan_sdl2.pc.in
