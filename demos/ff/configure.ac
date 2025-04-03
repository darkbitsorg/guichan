AC_PREREQ([2.60])

AC_INIT([guichanffdemo],[0.8.3],[olof@darkbits.org],[guichanffdemo])

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

AC_CHECK_LIB(SDL_image, IMG_Load, GUICHANFFDEMO_LIBS="$GUICHANFFDEMO_LIBS -lSDL_image",AC_MSG_ERROR([Could not find lib SDL_image!]))

AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, GUICHANFFDEMO_LIBS="$GUICHANFFDEMO_LIBS -lSDL_mixer",AC_MSG_ERROR([Could not find lib SDL_mixer!]))

AC_CHECK_LIB(guichan, gcnGuichanVersion, GUICHANFFDEMO_LIBS="$GUICHANFFDEMO_LIBS -lguichan -lguichan_sdl",AC_MSG_ERROR([Could not find lib Guichan!]))

AC_SUBST([GUICHANFFDEMO_LIBS])

AC_CONFIG_FILES([
Makefile
images/Makefile
sound/Makefile
])
AC_OUTPUT
