SUBDIRS = widgets hge openlayer contrib

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

libguichanincludedir = $(includedir)/guichan

libguichaninclude_HEADERS =	\
	actionevent.hpp		\
	actionlistener.hpp	\
	allegro.hpp		\
	cliprectangle.hpp	\
	color.hpp		\
	containerevent.hpp	\
	containerlistener.hpp	\
	deathlistener.hpp	\
	defaultfont.hpp		\
	event.hpp		\
	exception.hpp		\
	focushandler.hpp	\
	focuslistener.hpp	\
	font.hpp		\
	genericinput.hpp	\
	glut.hpp		\
	graphics.hpp		\
	gui.hpp			\
	image.hpp		\
	imagefont.hpp		\
	imageloader.hpp		\
	input.hpp		\
	inputevent.hpp		\
	key.hpp			\
	keyevent.hpp		\
	keyinput.hpp		\
	keylistener.hpp		\
	listmodel.hpp		\
	mouseevent.hpp		\
	mouseinput.hpp		\
	mouselistener.hpp	\
	opengl.hpp		\
	platform.hpp		\
	rectangle.hpp		\
	selectionevent.hpp	\
	selectionlistener.hpp	\
	sdl.hpp			\
	text.hpp		\
	widget.hpp		\
	widgetlistener.hpp

EXTRA_DIST = 			\
	hge.hpp			\
	openlayer.hpp
