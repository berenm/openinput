/*
 * linuxjoy.h: GNU/Linux joystick driver
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

#ifndef _OPENINPUT_LINUXJOY_H_
#define _OPENINPUT_LINUXJOY_H_

/* ******************************************************************** */

// Bootstrap entries
sint linuxjoy_avail();
oi_device *linuxjoy_device();

/* ******************************************************************** */

// Device entries
sint linuxjoy_init(oi_device *dev, char *window_id, uint flags);
sint linuxjoy_enable(oi_device *dev, sint on);
sint linuxjoy_destroy(oi_device *dev);
void linuxjoy_process(oi_device *dev);
sint linuxjoy_reset(oi_device *dev);

/* ******************************************************************** */

// Misc local functions
int linuxjoy_getfd(uchar num);

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @brief Linux joystick driver private instance data
 *
 * Private data for a single joystick device, such as the
 * file handle and information about the hat/ball/button/axis
 * mappings etc.
 *
 * Note that this driver can handle several joysticks!
 */
typedef struct linuxjoy_private {
  sint fd;                       /**< File descriptor */
  uchar id;                      /**< Device index, ie. the X in /dev/input/jsX */
  char *name;                    /**< Custom device name */
  char *desc;                    /**< Custom devuce description */
  uchar num_axes;                /**< Number of 2d-axes */
  uchar num_buttons;             /**< Number of buttons */
  uchar num_balls;               /**< Number of trackballs */
  uchar num_hats;                /**< Number of hats */
} linuxjoy_private;

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @{
 */
#define DLJS_MAX_DEVS 32         /**< Max joystick devices */
#define DLJS_DESC_SIZE 128       /**< Length of a joystick description (kernel) */
#define DLJS_NAME_SIZE 16        /**< Length of a joystick name */
/** @} */

/* ******************************************************************** */

#endif
