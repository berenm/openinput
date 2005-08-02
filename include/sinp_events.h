/*
 * sinp_events.h : Event structure types
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

#ifndef _SINP_EVENTS_H_
#define _SINP_EVENTS_H_

#ifndef _SINP_H_
#error Do not include this file directly - use sinp.h
#endif

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
  SINP_DISCOVERY                  = 10, // Device driver available
  SINP_ACTION                     = 11  // Action event (actionmap)
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
#define SINP_MASK_ACTIVE          SINP_EVENT_MASK(SINP_ACTIVE)
#define SINP_MASK_RESIZE          SINP_EVENT_MASK(SINP_RESIZE)
#define SINP_MASK_EXPOSE          SINP_EVENT_MASK(SINP_EXPOSE)
#define SINP_MASK_WINDOW          (SINP_EVENT_MASK(SINP_ACTIVE) | \
				   SINP_EVENT_MASK(SINP_RESIZE) | \
				   SINP_EVENT_MASK(EXPOSE))
#define SINP_MASK_DISCOVERY       SINP_EVENT_MASK(SINP_DISCOVERY)
#define SINP_MASK_ACTION          SINP_EVENT_MASK(SINP_ACTION)
#define SINP_MASK_QUIT            SINP_EVENT_MASK(SINP_QUIT)

/* ********************************************************************
 * Special event structures
 ******************************************************************** */

// Device discovery event
typedef struct sinp_discovery_event {
  uchar type;               // SINP_DISCOVERY
  uchar device;             // Device index
  char *name;               // Short name
  char *description;        // Long description
  uint provides;            // Provide mask
} sinp_discovery_event;

// Application visibility event
typedef struct sinp_active_event {
  uchar type;               // SINP_ACTIVE
  uchar device;             // Device index
  uchar gain;               // Focus was 0:lost 1:gained
  uint state;               // Mask of focus state
} sinp_active_event;

// Keyboard event
typedef struct sinp_keyboard_event {
  uchar type;               // SINP_KEYUP or SINP_KEYDOWN
  uchar device;             // Device index
  uchar state;              // Key is 0:up 1:down
  sinp_keysym keysym;       // Key symbol
} sinp_keyboard_event;

// Mouse move event
typedef struct sinp_mousemove_event {
  uchar type;               // SINP_MOUSEMOVE
  uchar device;             // Device index
  uchar state;              // Button states
  ushort x;                 // Absolute x coordinate
  ushort y;                 // Absolute y coordinate
  sshort relx;              // Relative x movement
  sshort rely;              // Relative y movement
} sinp_mousemove_event;

// Mouse button event
typedef struct sinp_mousebutton_event {
  uchar type;               // SINP_MOUSEBUTTONUP or SINP_MOUSEBUTTONDOWN
  uchar device;             // Device index
  uchar button;             // Mouse button index
  uchar state;              // Button states
  ushort x;                 // Absolute x coordinate at event time
  ushort y;                 // Absolute y coordinate at event time
} sinp_mousebutton_event;

// Application window resize event
typedef struct sinp_resize_event {
  uchar type;               // SINP_RESIZE
  uchar device;             // Device index
  uint width;               // New window width
  uint height;              // New window height
} sinp_resize_event;

// Application window needs redraw
typedef struct sinp_expose_event {
  uchar type;               // SINP_EXPOSE
} sinp_expose_event;

// Quit event
typedef struct sinp_quit_event {
  uchar type;               // SINP_QUIT
} sinp_quit_event;

// Action event
typedef struct sinp_action_event {
  uchar type;               // SINP_ACTION
  uchar device;             // Device index
  uint actionid;            // User-defined actionid
  uchar state;              // State (pressed/released)
  sint data1;               // Default data slot   (1d device: x coord)
  sint data2;               // Secondary data slot (2d device: y coord)
  sint data3;               // Tertiary data slot  (3d device: z coord)
} sinp_action_event;

/* ********************************************************************
 * The generic (united) event structure
 ******************************************************************** */

// The united event structure
typedef union {
  uchar type;
  sinp_active_event active;
  sinp_keyboard_event key;
  sinp_mousemove_event move;
  sinp_mousebutton_event button;
  sinp_resize_event resize;
  sinp_expose_event expose;
  sinp_quit_event quit;
  sinp_discovery_event discover;
  sinp_action_event action;
} sinp_event;

/* ******************************************************************** */

#endif
