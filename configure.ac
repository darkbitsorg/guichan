AC_PREREQ([2.60])

AC_INIT([guichan],[0.9.0],[olof.naessen@gmail.com],[guichan])

AC_CANONICAL_TARGET
AC_SUBST(target)
AC_CONFIG_MACRO_DIR([m4])

ALLEGRO="disabled"
OPENGL="disabled"
SDL="disabled"
SDLIMAGE="disabled"
dnl GLUT="disabled"
dnl X="disabled"

GUICHAN_MAJOR_VERSION=0
GUICHAN_MINOR_VERSION=9
GUICHAN_MICRO_VERSION=0
GUICHAN_INTERFACE_AGE=0
GUICHAN_BINARY_AGE=0
GUICHAN_VERSION=$GUICHAN_MAJOR_VERSION.$GUICHAN_MINOR_VERSION.$GUICHAN_MICRO_VERSION

AC_SUBST(GUICHAN_MAJOR_VERSION)
AC_SUBST(GUICHAN_MINOR_VERSION)
AC_SUBST(GUICHAN_MICRO_VERSION)
AC_SUBST(GUICHAN_INTERFACE_AGE)
AC_SUBST(GUICHAN_BINARY_AGE)
AC_SUBST(GUICHAN_VERSION)

LT_RELEASE=$GUICHAN_MAJOR_VERSION.$GUICHAN_MINOR_VERSION
LT_CURRENT=`expr $GUICHAN_MICRO_VERSION - $GUICHAN_INTERFACE_AGE`
LT_REVISION=$GUICHAN_INTERFACE_AGE
LT_AGE=`expr $GUICHAN_BINARY_AGE - $GUICHAN_INTERFACE_AGE`

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([include/config.hpp])

AC_CONFIG_SRCDIR([src/gui.cpp])

AC_PROG_CXX
AC_PROG_CC

PKG_PROG_PKG_CONFIG

LT_INIT([win32-dll])
AC_SUBST([LIBTOOL_DEPS])
AC_PROG_MAKE_SET
AC_LANG([C++])
AC_PROG_INSTALL

CXXFLAGS="$CXXFLAGS -Wall -Wno-unused -DGUICHAN_BUILD"

dnl NoX()
dnl {
dnl   AC_MSG_WARN(Xlib support skipped when Xlib not found.)
dnl   X="no"
dnl }

dnl HaveX()
dnl {
dnl   X="yes"
dnl   X_LIBS="$X_LIBS -L$x_libraries -lX11"
dnl   GUICHAN_EXTRADIRS="$GUICHAN_EXTRADIRS x"
dnl }

NoOpenGL()
{
  AC_MSG_WARN(OpenGL support skipped when OpenGL not found.)
  OPENGL="no"
  AM_CONDITIONAL([WITH_OPENGL], [false])
}

HaveOpenGL()
{
  OPENGL="yes"
  AM_CONDITIONAL([WITH_OPENGL], [true])
}

ForceOpenGL()
{
  AC_MSG_WARN(OpenGL support forced.)
  OPENGL="forced"
  AM_CONDITIONAL([WITH_OPENGL], [true])
}

dnl NoGLUT()
dnl {
dnl   AC_MSG_WARN(GLUT support skipped when GLUT not found.)
dnl   GLUT="no"
dnl }

dnl HaveGLUT()
dnl {
dnl   GLUT="yes"
dnl   GUICHAN_EXTRADIRS="$GUICHAN_EXTRADIRS glut"
dnl }

dnl ForceGLUT()
dnl {
dnl   AC_MSG_WARN(GLUT support forced.)
dnl   GLUT="forced"
dnl   GUICHAN_EXTRADIRS="$GUICHAN_EXTRADIRS glut"
dnl }

NoSDLImage()
{
  AC_MSG_WARN(SDL_image support skipped when SDL_image not found.)
  SDLIMAGE="no"
}

HaveSDLImage()
{
  SDLIMAGE="yes"
  # Try using pkg-config first, fall back to manual configuration if not available
  PKG_CHECK_MODULES([SDL], [sdl SDL_image], [], [
    SDL_LIBS="$SDL_LIBS -lSDL_image"
    SDL_CFLAGS="`sdl-config --cflags`"
  ])
  AC_SUBST(SDL_CFLAGS)
  AC_SUBST(SDL_LIBS)
}

ForceSDLImage()
{
  AC_MSG_WARN(SDL Image support forced.)
  SDLIMAGE="forced"
}

HaveSDL()
{
  AM_CONDITIONAL([WITH_SDL], [true])
}

ForceSDL()
{
  AC_MSG_WARN(SDL support forced.)
  AM_CONDITIONAL([WITH_SDL], [true])
}

NoAllegro()
{
  AC_MSG_WARN(Allegro support skipped when Allegro not found.)
  AM_CONDITIONAL([WITH_ALLEGRO], [false])
}

HaveAllegro()
{
  ALLEGRO="yes"
  AM_CONDITIONAL([WITH_ALLEGRO], [true])
}

ForceAllegro()
{
  AC_MSG_WARN(Allegro support forced.)
  ALLEGRO="forced"
  AM_CONDITIONAL([WITH_ALLEGRO], [true])
}

dnl AC_ARG_ENABLE(x,
dnl [  --enable-x	Enable Xlib support [default=no]],
dnl                , enable_x=no)
dnl if test x$enable_x = xyes; then
dnl   AC_PATH_X
dnl   if test "x$no_x" = "xyes"; then
dnl     NoX
dnl   else
dnl   dnl   AC_CHECK_HEADERS([X11/Xlib.h])
dnl   dnl   AC_CHECK_LIB([X11], [XOpenDisplay], HaveX, NoX)
dnl     HaveX
dnl   fi
dnl fi

AC_ARG_ENABLE(opengl,
[  --enable-opengl	Enable OpenGL support [default=yes]],
               , enable_opengl=yes)
AC_ARG_ENABLE(force_opengl,
[  --enable-force-opengl     Force OpenGL support (skip checks for OpenGL) [default=no]],
              , enable_force_opengl=no)
if test x$enable_opengl = xyes; then
  if test x$enable_force_opengl = xno; then
    AC_CHECK_HEADERS(GL/gl.h)
    AC_CHECK_LIB([GL], [glBegin], HaveOpenGL, NoOpenGL)
  else
    ForceOpenGL
  fi
fi

dnl AC_ARG_ENABLE(glut,
dnl [  --enable-glut	Enable GLUT support [default=yes]],
dnl                , enable_glut=yes)
dnl AC_ARG_ENABLE(force_glut,
dnl [  --enable-force-glut     Force GLUT support (skip checks for GLUT) [default=no]],
dnl               , enable_force_glut=no)
dnl if test x$enable_glut = xyes; then
dnl   if test x$enable_force_glut = xno; then
dnl     AC_CHECK_HEADERS(GL/glut.h)
dnl     AC_CHECK_LIB([glut], [glutInit], HaveGLUT, NoGLUT)
dnl   else
dnl     ForceGLUT
dnl   fi
dnl fi

AC_ARG_ENABLE(sdlimage,
[  --enable-sdlimage	Enable SDL_image support [default=yes]],
              , enable_sdlimage=yes)
AC_ARG_ENABLE(force_sdlimage,
[  --enable-force-sdlimage     Force SDL Image support (skip checks for SDL Image) [default=no]],
              , enable_force_sdlimage=no)
if test x$enable_sdlimage = xyes; then
  if test x$enable_force_sdlimage = xno; then
    AC_CHECK_HEADERS([SDL/SDL_image.h])
    AC_CHECK_LIB([SDL_image], [IMG_Load], HaveSDLImage, NoSDLImage)
  else
    ForceSDLImage
  fi
fi

AC_ARG_ENABLE(allegro,
[  --enable-allegro	  Enable Allegro support [default=yes]],
              , enable_allegro=yes)
AC_ARG_ENABLE(force_allegro,
[  --enable-force-allegro     Force Allegro support (skip checks for Allegro) [default=no]],
              , enable_force_allegro=no)
if test x$enable_allegro = xyes; then
  if test x$enable_force_allegro = xno; then
    AC_CHECK_HEADERS([allegro.h])
    AC_CHECK_PROG(HAVE_ALLEGRO, allegro-config, yes)
    if test "x$HAVE_ALLEGRO" != "xyes"; then
      NoAllegro
    else
      HaveAllegro
    fi
  else
    ForceAllegro
  fi
fi

AC_ARG_ENABLE(sdl,
[  --enable-sdl	  Enable SDL support [default=yes]],
             , enable_sdl=yes)
AC_ARG_ENABLE(force_sdl,
[  --enable-force-sdl     Force SDL support (skip checks for SDL) [default=no]],
              , enable_force_sdl=no)
if test x$enable_sdl = xyes; then
  if test x$enable_force_sdl = xno; then
    AC_CHECK_HEADERS(SDL/SDL.h)
    AC_CHECK_PROG(HAVE_SDL, sdl-config, yes)
    if test "x$HAVE_SDL" != "xyes"; then
      SDL="no"
    else
      SDL="yes"
    fi
  else
    SDL="forced"
    ForceSDL
  fi
fi

if test $SDL = "yes"; then
  if test $SDLIMAGE = "no"; then
    SDL="no"
    AC_MSG_WARN(SDL support skipped when SDL_image not found.)
    AM_CONDITIONAL([WITH_SDL], [false])
  else
    HaveSDL
  fi
else
  SDL="no"
  AC_MSG_WARN(SDL support skipped when SDL not found.)
  AM_CONDITIONAL([WITH_SDL], [false])
fi

AC_CONFIG_FILES([
Makefile
guichan.pc
examples/Makefile
include/Makefile
include/guichan/Makefile
include/guichan/allegro/Makefile
include/guichan/contrib/Makefile
include/guichan/contrib/allegro/Makefile
include/guichan/contrib/opengl/Makefile
include/guichan/contrib/sdl/Makefile
include/guichan/contrib/widgets/Makefile
include/guichan/hge/Makefile
include/guichan/opengl/Makefile
include/guichan/openlayer/Makefile
include/guichan/sdl/Makefile
include/guichan/widgets/Makefile
src/Makefile
src/allegro/Makefile
src/contrib/Makefile
src/contrib/allegro/Makefile
src/contrib/opengl/Makefile
src/contrib/sdl/Makefile
src/contrib/widgets/Makefile
src/hge/Makefile
src/opengl/Makefile
src/opengl/guichan_opengl.pc
src/openlayer/Makefile
src/sdl/Makefile
src/sdl/guichan_sdl.pc
src/widgets/Makefile])
AC_OUTPUT
echo ""
echo "IMPORTANT! You are about install a BETA version of Guichan"
echo "which most likely will lose binary compatibility with the"
echo "future stable release. This release should only be used"
echo "for testing."
echo ""
echo "-------------------------------"
echo "Guichan ready for compilation!"
echo "-------------------------------"
echo "* Allegro   = "$ALLEGRO
echo "* HGE       = no (Windows specific and cannot be built)"
echo "* OpenGL    = "$OPENGL
echo "* OpenLayer = no (Requires static build and cannot be built)"
echo "* SDL       = "$SDL
echo "* SDL Image = "$SDLIMAGE
echo "--------------------------------"
