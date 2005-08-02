/*
 * sinp_types.h : Basic type definitions
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

#ifndef _SINP_TYPES_H_
#define _SINP_TYPES_H_

#ifndef _SINP_H_
#error Do not include this file directly - use sinp.h
#endif

/* ********************************************************************
 * Basic numeric types
 ******************************************************************** */

// Basic integer types
typedef unsigned char             uchar;
typedef signed char               schar;
typedef unsigned short            ushort;
typedef signed short              sshort;
typedef unsigned int              uint;
typedef signed int                sint;

/* ********************************************************************
 * Alternate boolean definition
 ******************************************************************** */

// On/off/query 
typedef enum {
  SINP_ENABLE,                    // True/enable
  SINP_DISABLE,                   // False/disable
  SINP_QUERY                      // Don't change, return current
} sinp_bool;

/* ********************************************************************
 * Various defines for the library, eg. mouse buttons and error codes
 ******************************************************************** */

// Init string parameters for window_id
#define SINP_I_CONN              'c' // Server connection handle
#define SINP_I_SCRN              's' // Screen handle
#define SINP_I_WINID             'w' // Window handle

// Device provide flags
#define SINP_PRO_UNKNOWN          1 // Unknown/test type device
#define SINP_PRO_KEYBOARD         2 // Keyboard input device
#define SINP_PRO_MOUSE            4 // Pointer input device (mouse)
#define SINP_PRO_JOYSTICK         8 // Joystick input device
#define SINP_PRO_WINDOW          16 // Window stuff (move/state/size)

// Application activation
#define SINP_FOCUS_MOUSE          1 // Window has mouse focus
#define SINP_FOCUS_INPUT          2 // Window has keyboard (input) focus
#define SINP_FOCUS_VISIBLE        4 // Window is visible

// Initialization flags for sinp_init
#define SINP_FLAG_NOWINDOW        1 // Do not hook into window

// Error codes
#define SINP_ERR_OK               0 // All ok
#define SINP_ERR_NO_DEVICE        1 // Wrong device
#define SINP_ERR_INDEX            2 // Index query out of bounds
#define SINP_ERR_NOT_IMPLEM       4 // Not implemented
#define SINP_ERR_DEV_EXIST        5 // Device already initialized
#define SINP_ERR_PARAM            6 // Invalid parameter
#define SINP_ERR_NO_NAME          7 // No such event name exists
#define SINP_ERR_NOT_UNIQUE       8 // Index was not unique
 
/* ******************************************************************** */

#endif
