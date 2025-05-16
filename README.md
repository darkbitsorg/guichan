# Guichan

*© 2004-2009 Olof Naessén*

> **IMPORTANT!**
> Guichan is currently in a beta stage. This means that the API is not fixed and
> may change in the future. Every API change will break compatibility between
> older versions of Guichan.

## Introduction to Guichan

Guichan is a C++ GUI library designed for games. It comes with a standard set of
*widgets* and can use several different objects for displaying graphics and
capturing user input. Guichan has an abstract design which allows users to use
different objects for displaying graphics and grabbing user input.

Guichan ships with a number of extensions that implement the graphics and input
interfaces for certain well known libraries (like SDL and Allegro). Due to
Guichan's extendible nature, new objects can also be created by the developer to
suit their needs.

As Guichan is designed for games, it lacks many features of more advanced GUI
libraries (such as GTK and Qt). However, a game developer may not normally
require all the advanced features of the larger GUI libraries. Guichan aims to
keep things small and simple, thus avoiding the complication and large
dependencies found with some other GUI libraries. Guichan is small but contains
all basic GUI features, making it ideal for games.

Guichan allows the developer control over the initialization of external
libraries, giving a degree of freedom which is almost required in game
development, but not provided by some GUI libraries. Guichan should not be
considered a full GUI library but rather a tool or helper library for game
development.

The basic nature of Guichan means that it comes with a small number of default
widgets. There exists no themes or theme handling for Guichan, as their
implementation would increase the size and complexity of the library
unnecessarily. Instead, the developer creates their own new widgets by
inheriting from the standard widgets or by overloading standard widget
functions. This makes Guichan much more flexible for customization than
theme-based GUIs, and more natural to use for C++ developers, as they need only
learn the API instead of a whole new theming system.

## More Information and Downloads

For more general information about Guichan and/or downloads related to Guichan,
please visit the [Guichan GitHub page](https://github.com/darkbitsorg/guichan).

## Getting Help with Guichan

If you need help with Guichan, the best way to proceed is to ask questions
through [GitHub issues](https://github.com/darkbitsorg/guichan/issues/new).

## Getting Involved with Guichan Development

If you want to get involved with Guichan development, the best way to proceed is
to suggest improvements to Guichan through GitHub issues or pull requests.
