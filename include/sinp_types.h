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
typedef unsigned char       uchar;
typedef signed char         schar;
typedef unsigned short      ushort;
typedef signed short        sshort;
typedef unsigned int        uint;
typedef signed int          sint;

/* ********************************************************************
 * Event types and masks
 ******************************************************************** */

// Event types
typedef enum {
  SINP_NOEVENT                    = 0,  // No event
  SINP_KEYUP                      = 1,  // Key released
  SINP_KEYDOWN                    = 2,  // Key pressed
  SINP_MOUSEMOVE                  = 3,  // Mouse motion
  SINP_MOUSEBUTTONUP              = 4,  // Button pressed
  SINP_MOUSEBUTTONDOWN            = 5,  // Button released
  SINP_ACTIVE                     = 6,  // App. focus gain/loss
  SINP_RESIZE                     = 7,  // App. window resize
  SINP_EXPOSE                     = 8,  // App. needs redraw
  SINP_QUIT                       = 9,  // Quit requested
  SINP_DISCOVERY                  = 10  // Device driver available
} sinp_type;
  
// Event masks
#define SINP_EVENT_MASK(x) (1<<(x))
#define SINP_MASK_ALL 0xffffffff
#define SINP_MASK_KEYUP           SINP_EVENT_MASK(SINP_KEYUP)
#define SINP_MASK_KEYDOWN         SINP_EVENT_MASK(SINP_KEYDOWN)
#define SINP_MASK_MOUSEMOVE       SINP_EVENT_MASK(SINP_MOUSEMOVE)
#define SINP_MASK_MOUSEBUTTONUP   SINP_EVENT_MASK(SINP_MOUSEBUTTONUP)
#define SINP_MASK_MOUSEBUTTONDOWN SINP_EVENT_MASK(SINP_MOUSEBUTTONDOWN)
#define SINP_MASK_MOUSE           (SINP_EVENT_MASK(SINP_MOUSEMOVE) | \
				   SINP_EVENT_MASK(SINP_MOUSEBUTTONUP) | \
				   SINP_EVENT_MASK(SINP_MOUSEBUTTONDOWN))
#define SINP_MASK_RESIZE          SINP_EVENT_MASK(SINP_RESIZE)
#define SINP_MASK_EXPOSE          SINP_EVENT_MASK(SINP_EXPOSE)
#define SINP_MASK_ACTIVE          SINP_EVENT_MASK(SINP_ACTIVE)
#define SINP_MASK_QUIT            SINP_EVENT_MASK(SINP_QUIT)

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
 * Mouse buttons
 ******************************************************************** */

// The buttons
#define SINP_BUTTON_LEFT          1
#define SINP_BUTTON_MIDDLE        2
#define SINP_BUTTON_RIGHT         3
#define SINP_WHEEL_UP             4
#define SINP_WHEEL_DOWN           5

// Masks for buttons
#define SINP_BUTTON_MASK(x) (1<<(x))
#define SINP_BUTTON_LEFTMASK      SINP_BUTTON_MASK(SINP_BUTTON_LEFT)
#define SINP_BUTTON_MIDMASK       SINP_BUTTON_MASK(SINP_BUTTON_MIDDLE)
#define SINP_BUTTON_RIGHTMASK     SINP_BUTTON_MASK(SINP_BUTTON_RIGHT)

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

// Error codes
#define SINP_ERR_OK               0 // All ok
#define SINP_ERR_NO_DEVICE        1 // Wrong device
#define SINP_ERR_INDEX            2 // Index query out of bounds
#define SINP_ERR_NOT_IMPLEM       4 // Not implemented
#define SINP_ERR_DEV_EXIST        5 // Device already initialized
#define SINP_ERR_PARAM            6 // Invalid parameter

/* ******************************************************************** */

#endif
