/*
 * foo.h : Foo (test) utility functions (bootstrapping, etc.)
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

#ifndef _OPENINPUT_FOO_H_
#define _OPENINPUT_FOO_H_

/* ******************************************************************** */

// Bootstrap
sint foo_avail();
oi_device *foo_device();

// Device
sint foo_init(oi_device *dev, char *window_id, uint flags);
sint foo_destroy(oi_device *dev);
void foo_process(oi_device *dev);
sint foo_grab(oi_device *dev, sint on);
sint foo_hidecursor(oi_device *dev, sint on);
sint foo_warp(oi_device *dev, sint x, sint y);
sint foo_winsize(oi_device *dev, sint *w, sint *h);
sint foo_reset(oi_device *dev);

/* ******************************************************************** */

/**
 * @ingroup DFoo
 * @brief Foo driver private instance data
 *
 * Private data is private for the particular
 * instance of the device driver.
 */
typedef struct foo_private {
    sint grabstatus;   /**< Pointer grabbed or free */
    sint cursorstatus; /**< Cursor shown of hidden */
    sint x;            /**< Cursor horizontal position */
    sint y;            /**< Cursor vertical position */
} foo_private;

/* ******************************************************************** */

#endif
