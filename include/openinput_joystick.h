/*
 * openinput_joystick.h : Definitions for joystick interface
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

#ifndef _OPENINPUT_JOYSTICK_H_
#define _OPENINPUT_JOYSTICK_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ******************************************************************** */

/**
 * @ingroup PTypes
 * @defgroup PJoyHats Joystick hat positions
 * @brief Joystick hat positions
 * @{
 */
#define OI_HAT_CENTER      0                          /**< Center */
#define OI_HAT_UP          1                          /**< Up */
#define OI_HAT_DOWN        2                          /**< Down */
#define OI_HAT_LEFT        4                          /**< Left */
#define OI_HAT_RIGHT       8                          /**< Right */
#define OI_HAT_UPLEFT      (OI_HAT_UP|OI_HAT_LEFT)    /**< Up + left */
#define OI_HAT_UPRIGHT     (OI_HAT_UP|OI_HAT_RIGHT)   /**< Up + right */
#define OI_HAT_DOWNLEFT    (OI_HAT_DOWN|OI_HAT_LEFT)  /**< Down + left */
#define OI_HAT_DOWNRIGHT   (OI_HAT_DOWN|OI_HAT_RIGHT) /**< Down + right */
/** @} */


/**
 * @ingroup PTypes
 * @defgroup PJoyTypes Joystick definitions
 * @brief Joystick buttons and constants
 * @{
 */
#define OI_JOY_NUM_DEVS      32       /**< Maximum number of joysticks */
#define OI_JOY_NUM_AXES      16       /**< Maximum buttons/axes */
#define OI_JOY_AXIS_MIN     -32768    /**< Minimum axis value */
#define OI_JOY_AXIS_MAX      326767   /**< Maximum axis value */
/** @} */


/**
 * @ingroup PJoyTypes
 * @brief Joystick axes classes
 *
 * Joysticks typically have different kinds of axes like
 * sticks, throttles and rudders.
 */
typedef enum {
  OI_AXIS_NONE                   = 0,                 /**< Not an axis */
  OI_AXIS_STICK                  = 1,                 /**< A classic stick */
  OI_AXIS_HAT                    = 2,                 /**< Positional button (possible pairing) */
  OI_AXIS_RUDDER                 = 3,                 /**< Yaw-stick */
  OI_AXIS_THROTTLE               = 4,                 /**< Throttle/trimmer */
  OI_AXIS_BALL                   = 5                  /**< Trackball (relative events, 2-axis) */
} oi_joyaxis;



/* ******************************************************************** */

#endif
