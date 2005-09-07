/*
 * win32.h : Microsoft Windows input device driver
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

#ifndef _OPENINPUT_WIN32_H_
#define _OPENINPUT_WIN32_H_

/* ******************************************************************** */

// Bootstrap
sint win32_avail();
oi_device *win32_device();

// Device
sint win32_init(oi_device *dev, char *window_id, uint flags);
sint win32_destroy(oi_device *dev);
void win32_process(oi_device *dev);
sint win32_grab(oi_device *dev, sint on);
sint win32_hidecursor(oi_device *dev, sint on);
sint win32_warp(oi_device *dev, sint x, sint y);
sint win32_winsize(oi_device *dev, sint *w, sint *h);
sint win32_reset(oi_device *dev);

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Foo driver private instance data
 *
 * Private data is private for the particular
 * instance of the device driver.
 */
typedef struct win32_private {
  sint x;            /**< Cursor horizontal position */
} win32_private;

/* ******************************************************************** */

#endif
