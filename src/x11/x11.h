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
sint x11_destroy(sinp_device *dev);
void x11_process(sinp_device *dev);
sint x11_grab(sinp_device *dev, sint on);
sint x11_hidecursor(sinp_device *dev, sint on);
sint x11_warp(sinp_device *dev, sint x, sint y);
sint x11_winsize(sinp_device *dev, sint *w, sint *h);

/* ******************************************************************** */

// Misc local functions
Cursor x11_mkcursor(Display *d, Window w);
sint x11_error(Display *d, XErrorEvent *e);
sint x11_fatal(Display *d);
inline sint x11_pending(Display *d);
inline void x11_dispatch(sinp_device *dev, Display *d);
inline sinp_keysym *x11_translate(Display *d, XKeyEvent *xkey,
				  KeyCode kc, sinp_keysym *keysym);

void x11_initkeymap();
void x11_keystate(sinp_device *dev, Display *d, char *keyvector);
void x11_modmasks(Display *d, sinp_device *dev);

/* ******************************************************************** */

// Private data structure
typedef struct x11_private {
  Display *disp;
  Window win;
  Screen *screen;
  Cursor cursor;
  Atom wm_delete_window;
  uint mask_lmeta;
  uint mask_rmeta;
  uint mask_lalt;
  uint mask_ralt;
  uint mask_num;
  uint mask_altgr;
} x11_private;

/* ******************************************************************** */

#endif
