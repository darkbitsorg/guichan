#!/bin/sh
#
echo "Generating build information using aclocal, autoheader, automake and autoconf"
echo "This may take a while ..."

# Make sure the m4 directory exists
mkdir -p m4

# Run libtoolize first to ensure libtool files are in place
libtoolize --force || { echo "libtoolize failed"; exit 1; }

# Regenerate configuration files
aclocal || { echo "aclocal failed"; exit 1; }
autoheader || { echo "autoheader failed"; exit 1; }
automake --foreign --include-deps --add-missing --copy || { echo "automake failed"; exit 1; }
autoreconf -i || { echo "autoreconf failed"; exit 1; }

echo "Now you are ready to run ./configure"
