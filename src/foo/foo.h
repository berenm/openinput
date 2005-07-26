/*
 * foo.h : Foo (test) utility functions (bootstrapping, etc.)
 *
 * This file is a part of libsinp - the simple input library.
 * Copyright (C) 2005  Jakob Kjaer <makob@makob.dk>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* ******************************************************************** */

#ifndef _SINP_FOO_H_
#define _SINP_FOO_H_

/* ******************************************************************** */

// Bootstrap
sint foo_avail();
sinp_device *foo_device();

// Device
sint foo_init(sinp_device *dev, char *window_id, uint flags);
sint foo_destroy(sinp_device *dev);
void foo_process(sinp_device *dev);
sint foo_grab(sinp_device *dev, sint on);
sint foo_hidecursor(sinp_device *dev, sint on);
sint foo_warp(sinp_device *dev, sint x, sint y);
sint foo_winsize(sinp_device *dev, sint *w, sint *h);

/* ******************************************************************** */

#endif
