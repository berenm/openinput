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

/* ******************************************************************** */

// Basic integer types
typedef unsigned char       uchar;
typedef signed char         schar;
typedef unsigned short      ushort;
typedef signed short        sshort;
typedef unsigned int        uint;
typedef signed int          sint;

/* ******************************************************************** */

// Device provide flags
#define SINP_PRO_UNKNOWN    1 // Unknown/test type device
#define SINP_PRO_KEYBOARD   2 // Keyboard input device
#define SINP_PRO_POINTER    4 // Pointer input device (mouse)
#define SINP_PRO_JOYSTICK   8 // Joystick input device
#define SINP_PRO_HIDECUR   16 // Cursor of device can be hidden

/* ******************************************************************** */

// Status flags
#define SINP_STA_DEAD       0 // Uninitialized
#define SINP_STA_ALIVE      1 // Device has been initialized
#define SINP_STA_RUNNING    2 // Device is running

/* ******************************************************************** */

// Event types
typedef enum {
  SINP_NOEVENT              = 0, // No event
  SINP_KEYUP                = 1, // Key released
  SINP_KEYDOWN              = 2, // Key pressed
  SINP_MOUSEMOVE            = 3, // Mouse motion
  SINP_MOUSEBUTTONUP        = 4, // Button pressed
  SINP_MOUSEBUTTONDOWN      = 5, // Button released
  SINP_ACTIVE               = 6, // App. focus gain/loss
  SINP_RESIZE               = 7, // App. window resize
  SINP_EXPOSE               = 8, // App. needs redraw
  SINP_QUIT                 = 9  // Quit requested
} sinp_type;
  
/* ******************************************************************** */

// Event masks
#define SINP_EVENT_MASK(x) (1<<(x))
#define SINP_MASK_ALL 0xffffffff
typedef enum {
  SINP_MASK_KEYUP           = SINP_EVENT_MASK(SINP_KEYUP),
  SINP_MASK_KEYDOWN         = SINP_EVENT_MASK(SINP_KEYDOWN),
  SINP_MASK_MOUSEMOVE       = SINP_EVENT_MASK(SINP_MOUSEMOVE),
  SINP_MASK_MOUSEBUTTONUP   = SINP_EVENT_MASK(SINP_MOUSEBUTTONUP),
  SINP_MASK_MOUSEBUTTONDOWN = SINP_EVENT_MASK(SINP_MOUSEBUTTONDOWN),
  SINP_MASK_MOUSE           = (SINP_EVENT_MASK(SINP_MOUSEMOVE) |
			       SINP_EVENT_MASK(SINP_MOUSEBUTTONUP) |
			       SINP_EVENT_MASK(SINP_MOUSEBUTTONDOWN)),
  SINP_MASK_RESIZE          = SINP_EVENT_MASK(SINP_RESIZE),
  SINP_MASK_EXPOSE          = SINP_EVENT_MASK(SINP_EXPOSE),
  SINP_MASK_ACTIVE          = SINP_EVENT_MASK(SINP_ACTIVE),
  SINP_MASK_QUIT            = SINP_EVENT_MASK(SINP_QUIT)
} sinp_event_masks;

/* ******************************************************************** */

// Mouse buttons
#define SINP_BUTTON_LEFT    1
#define SINP_BUTTON_MIDDLE  2
#define SINP_BUTTON_RIGHT   3
#define SINP_WHEEL_UP       4
#define SINP_WHEEL_DOWN     5

/* ******************************************************************** */

// Application activation
#define SINP_FOCUS_MOUSE    1
#define SINP_FOCUS_INPUT    2
#define SINP_FOCUS_ACTIVE   3

/* ******************************************************************** */

// Error codes
#define SINP_ERR_OK          0 // All ok
#define SINP_ERR_NO_DEVICE   1 // Wrong device
#define SINP_ERR_INDEX       2 // Index query out of bounds
#define SINP_ERR_DEV_DEAD    3 // Device was dead
#define SINP_ERR_NOT_IMPLEM  4 // Not implemented
#define SINP_ERR_DEV_EXIST   5 // Device already initialized
#define SINP_ERR_PARAM       6 // Invalid parameter
#define SINP_ERR_DEV_RUNNING 7 // Device already enabled/running

/* ******************************************************************** */

#endif
