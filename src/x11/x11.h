/*
 * x11.h : X11 utility functions (bootstrapping, etc.)
 *
 * This file is a part of the OpenInput library.
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

#ifndef _OPENINPUT_X11_H_
#define _OPENINPUT_X11_H_

/* ******************************************************************** */

// Bootstrap entries
sint x11_avail();
oi_device *x11_device();

/* ******************************************************************** */

// Device entries
sint x11_init(oi_device *dev, char *window_id, uint flags);
sint x11_destroy(oi_device *dev);
void x11_process(oi_device *dev);
sint x11_grab(oi_device *dev, sint on);
sint x11_hidecursor(oi_device *dev, sint on);
sint x11_warp(oi_device *dev, sint x, sint y);
sint x11_winsize(oi_device *dev, sint *w, sint *h);
sint x11_reset(oi_device *dev);

/* ******************************************************************** */

// Misc local functions
Cursor x11_mkcursor(Display *d, Window w);
sint x11_error(Display *d, XErrorEvent *e);
sint x11_fatal(Display *d);
inline sint x11_pending(Display *d);
inline void x11_dispatch(oi_device *dev, Display *d);
inline oi_keysym *x11_translate(Display *d, XKeyEvent *xkey,
				KeyCode kc, oi_keysym *keysym);
void x11_initkeymap();
void x11_keystate(oi_device *dev, Display *d, char *keyvector);
void x11_modmasks(Display *d, oi_device *dev);
inline void x11_relative_mouse(oi_device *dev, XEvent *xev);
inline schar x11_keyrepeat(Display *d, XEvent *evt);

/* ******************************************************************** */

/**
 * @ingroup DX11
 * @brief X11 driver private instance data
 *
 * Private data for the X11 driver. This include the
 * display, screen and window handles.
 */
typedef struct x11_private {
  Display *disp;             /**< Display handle */
  Window win;                /**< Window handle  */
  Screen *screen;            /**< Screen handle (unused) */
  Cursor cursor;             /**< The invisible cursor */
  Atom wm_delete_window;     /**< Close-window protocol atom */
  uint mask_lmeta;           /**< Variable mask for key */
  uint mask_rmeta;           /**< Variable mask for key */
  uint mask_lalt;            /**< Variable mask for key */
  uint mask_ralt;            /**< Variable mask for key */
  uint mask_num;             /**< Variable mask for key */
  uint mask_altgr;           /**< Variable mask for key */
  uchar relative;            /**< Relative mouse motion */
  int lastx;                 /**< Last mouse x positon */
  int lasty;                 /**< Last mouse y position */
  int width;                 /**< Window width */
  int height;                /**< Window height */
} x11_private;

/* ******************************************************************** */

/**
 * @ingroup DX11
 * @{
 */
#define SX11_GRAB 1          /**< Grabbed state flag */
#define SX11_HIDE 2          /**< Hidden state flag */
#define SX11_FUDGE 8         /**< Mouse fudge factor */
#define SX11_REP_THRESHOLD 2 /**< Key repeat threshold */
/** @} */

/* ******************************************************************** */

#endif
