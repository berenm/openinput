/*
 * linuxjoy_devs.h: Special joystick setups
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

#ifndef _OPENINPUT_LINUXJOY_DEVS_H_
#define _OPENINPUT_LINUXJOY_DEVS_H_

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @brief Special joysticks
 *
 * Structure to contain information about non-standard joysticks.
 */
struct linuxjoy_specials {
  const char *name;                      /**< Joystick kernel name */
  uint num_axes;                         /**< Number of axes */
  uint num_hats;                         /**< Number of hats */
  uint num_balls;                        /**< Number of trackballs */
} linuxjoy_specials[] = {
  //  Name                                               Axes, hats, balls
  { "MadCatz Panther XL",                                3,    2,    1 },
  { "SideWinder Precision Pro",                          4,    1,    0 },
  { "SideWinder 3D Pro",                                 4,    1,    0 },
  { "Microsoft SideWinder 3D Pro",                       4,    1,    0 },
  { "Microsoft SideWinder Dual Strike USB version 1.0",  2,    1,    0 },
  { "WingMan Interceptor",                               3,    3,    0 },
  { "WingMan Extreme Digital 3D",                        4,    1,    0 },
  { "Microsoft SideWinder Precision 2 Joystick",         4,    1,    0 },
  { "Logitech Inc. WingMan Extreme Digital 3D",          4,    1,    0 },
  { "Saitek Saitek X45",                                 6,    1,    0 }
};

/* ******************************************************************** */

#endif
