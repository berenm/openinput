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
inline void x11_relative_mouse(sinp_device *dev, XEvent *xev);

/* ******************************************************************** */

// Private data structure
typedef struct x11_private {
  Display *disp;         // Display
  Window win;            // Window we hook into
  Screen *screen;        // Screen (unused at the moment)
  Cursor cursor;         // The invisible cursor
  Atom wm_delete_window; // Close-window protocol atom
  uint mask_lmeta;       // Variable mask for key
  uint mask_rmeta;       // Variable mask for key
  uint mask_lalt;        // Variable mask for key
  uint mask_ralt;        // Variable mask for key
  uint mask_num;         // Variable mask for key
  uint mask_altgr;       // Variable mask for key
  uchar relative;        // Relative mouse motion
  int lastx;             // Last mouse x positon
  int lasty;             // Last mouse y position
  int width;             // Window width
  int height;            // Window height
} x11_private;

/* ******************************************************************** */

// Grabbed/hidden state - used in private->relative
#define SX11_GRAB 1
#define SX11_HIDE 2

// The SDL fudge factor optimization
#define SX11_FUDGE 8

/* ******************************************************************** */

#endif
