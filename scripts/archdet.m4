dnl archdet.m4 : "Nice and simple" autoconf architecture detection
dnl
dnl This file is a part of the OpenInput library.
dnl Copyright (C) 2005  Jakob Kjaer <makob@makob.dk>.
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

AC_DEFUN([ARCHDET], [
    AC_MSG_CHECKING([for short platform name])

    case "$target" in
        dnl GNU/Linux variants
        *-*-linux*|*-*-gnu*|*-*-k*bsd*-gnu)
	    case "$target" in

                *-*-linux*)
		    ARCH=linux ;;

                *-*-kfreebsd*-gnu)
		    ARCH=kfreebsd-gnu ;;

                *-*-knetbsd*-gnu)
		    ARCH=knetbsd-gnu ;;

                *-*-kopenbsd*-gnu)
		    ARCH=kopenbsd-gnu ;;

                *-*-gnu*)
		    ARCH=gnu ;;
            esac
	    ;;

	dnl BSDi
        *-*-bsdi*)
	    ARCH=bsdi ;;

	dnl FreeBSD
	*-*-freebsd*)
	    ARCH=freebsd ;;

        dnl NetBSD
	*-*-netbsd*)
	    ARCH=netbsd ;;

	dnl OpenBSD
	*-*-openbsd*)
	    ARCH=openbsd ;;
 
	dnl SystemV5
	*-*-sysv5*)
	    ARCH=sysv5 ;;

	dnl Sun Solaris
	*-*-solaris*)
	    ARCH=solaris ;;

	dnl Windows
	*-*-cygwin*|*-*-mingw32*)
	    ARCH=win32 ;;

	dnl MacOS X
	*-*-darwin*)
	    ARCH=macosx ;;

	*)
	    AC_MSG_ERROR([Unsupported/unknown system])
	    ;;
    esac
    AC_SUBST(ARCH)

    AC_MSG_RESULT($ARCH)
])

