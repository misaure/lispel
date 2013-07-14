#!/bin/sh

aclocal
libtoolize --force
automake --add-missing
automake
autoconf

