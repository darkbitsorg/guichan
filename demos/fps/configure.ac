AC_PREREQ([2.60])

AC_INIT([guichanfpsdemo],[0.8.3],[olof@darkbits.org],[guichanfpsdemo])

AC_CANONICAL_TARGET
AC_SUBST(target)

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([include/config.hpp])

AC_PROG_CXX
AC_PROG_CC

AC_PROG_MAKE_SET
AC_LANG([C++])
AC_PROG_INSTALL

AC_CHECK_PROG(HAS_SDL, sdl-config, yes)
if test "x$HAS_SDL" != "xyes"; then
   AC_MSG_ERROR([ SDL Simple Direct Layer library not found! You can get a copy at http://www.libsdl.org/])
fi
CPPFLAGS="$CFLAGS `sdl-config --cflags`"
LDFLAGS="$LDFLAGS `sdl-config --libs`"

AC_CHECK_LIB(SDL_image, IMG_Load, GUICHANFPSDEMO_LIBS="$GUICHANFPSDEMO_LIBS -lSDL_image",AC_MSG_ERROR([Could not find lib SDL_image (SDL)!]))

AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, GUICHANFPSDEMO_LIBS="$GUICHANFPSDEMO_LIBS -lSDL_mixer",AC_MSG_ERROR([Could not find lib SDL_mixer (SDL_mixer)!]))

AC_CHECK_LIB(GL, glAccum, GUICHANFPSDEMO_LIBS="$GUICHANFPSDEMO_LIBS -lGL",AC_MSG_ERROR([Could not find lib OpenGL (GL)!]))

AC_CHECK_LIB(GLU, gluBeginCurve, GUICHANFPSDEMO_LIBS="$GUICHANFPSDEMO_LIBS -lGLU",AC_MSG_ERROR([Could not find lib OpenGL Utils (GLU)!]))

AC_CHECK_LIB(guichan, gcnGuichanVersion, GUICHANFPSDEMO_LIBS="$GUICHANFPSDEMO_LIBS -lguichan -lguichan_sdl -lguichan_opengl",AC_MSG_ERROR([Could not find lib Guichan (guichan)!]))

AC_SUBST([GUICHANFPSDEMO_LIBS])

AC_CONFIG_FILES([
Makefile
images/Makefile
sound/Makefile
])
AC_OUTPUT
