# We include the current directory after 'widgets' but before the various
# backends, because libguichan depends on libguichan_widgets, whereas the
# backends depend on libguichan.
SUBDIRS = widgets . hge openlayer contrib

# Conditionally add subdirectories based on available libraries
if WITH_ALLEGRO
SUBDIRS += allegro
endif

if WITH_SDL
SUBDIRS += sdl
endif

if WITH_OPENGL
SUBDIRS += opengl
endif

AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libguichan.la

libguichan_la_LIBADD = widgets/libguichan_widgets.la
libguichan_la_LDFLAGS = -no-undefined -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libguichan_la_DEPENDENCIES = \
	widgets/libguichan_widgets.la

# The Gui-chan library sources
libguichan_la_SOURCES = 	\
	actionevent.cpp		\
	cliprectangle.cpp	\
	color.cpp		\
	containerevent.cpp	\
	defaultfont.cpp		\
	event.cpp		\
	exception.cpp		\
	focushandler.cpp	\
	font.cpp		\
	genericinput.cpp	\
	graphics.cpp		\
	gui.cpp			\
	guichan.cpp		\
	image.cpp		\
	imagefont.cpp		\
	inputevent.cpp		\
	key.cpp			\
	keyevent.cpp		\
	keyinput.cpp		\
	mouseevent.cpp		\
	mouseinput.cpp		\
	rectangle.cpp		\
	selectionevent.cpp	\
	text.cpp		\
	widget.cpp
