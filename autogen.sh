#!/bin/sh
#
echo "Generating build information using aclocal, autoheader, automake and autoconf"
echo "This may take a while ..."

# Make sure the m4 directory exists
mkdir -p m4

# Regenerate configuration files
aclocal
autoheader
automake --foreign --include-deps --add-missing --copy
libtoolize --force
autoreconf -i

# Run configure for this platform
#./configure $*
echo "Now you are ready to run ./configure"
