/*
 * x11.h : X11 utility functions (bootstrapping, etc.)
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

#ifndef _SINP_X11_H_
#define _SINP_X11_H_

/* ******************************************************************** */

// Bootstrap entries
sint x11_avail();
sinp_device *x11_device();

/* ******************************************************************** */

// Device entries
sint x11_init(sinp_device *dev, char *window_id, uint flags);
sint x11_enable(sinp_device *dev, sint on);
sint x11_destroy(sinp_device *dev);
void x11_process(sinp_device *dev);
sint x11_grab(sinp_device *dev, uint mask);
sint x11_hidecursor(sinp_device *dev, sint on);

/* ******************************************************************** */

// Misc local functions
Cursor x11_mkcursor(Display *d, Window w);
sint x11_error(Display *d, XErrorEvent *e);
sint x11_fatal(Display *d);
inline sint x11_pending(Display *d);
inline void x11_dispatch(Display *d);

/* ******************************************************************** */

#endif
