# This file is part of Autoconf.                       -*- Autoconf -*-
# Copyright (c) 2006 - Jakob Kjaer <makob@makob.dk>
# This file is distributed under the same terms as the Autoconf macro files.

# Print the directory-part of a path by using shell-expansion
AC_DEFUN([DIRNAME_EXPR],
         [[expr ".$1" : '\(\.\)[^/]*$' \| "x$1" : 'x\(.*\)/[^/]*$']])
