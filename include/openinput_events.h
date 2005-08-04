/*
 * openinput_events.h : Event structure types
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

#ifndef _OPENINPUT_EVENTS_H_
#define _OPENINPUT_EVENTS_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ********************************************************************
 * Event types and masks
 ******************************************************************** */

// Event types
typedef enum {
  OI_NOEVENT                    = 0,  // No event
  OI_KEYUP                      = 1,  // Key released
  OI_KEYDOWN                    = 2,  // Key pressed
  OI_MOUSEMOVE                  = 3,  // Mouse motion
  OI_MOUSEBUTTONUP              = 4,  // Button pressed
  OI_MOUSEBUTTONDOWN            = 5,  // Button released
  OI_ACTIVE                     = 6,  // App. focus gain/loss
  OI_RESIZE                     = 7,  // App. window resize
  OI_EXPOSE                     = 8,  // App. needs redraw
  OI_QUIT                       = 9,  // Quit requested
  OI_DISCOVERY                  = 10, // Device driver available
  OI_ACTION                     = 11  // Action event (actionmap)
} oi_type;
  
// Event masks
#define OI_EVENT_MASK(x) (1<<(x))
#define OI_MASK_ALL 0xffffffff
#define OI_MASK_KEYUP           OI_EVENT_MASK(OI_KEYUP)
#define OI_MASK_KEYDOWN         OI_EVENT_MASK(OI_KEYDOWN)
#define OI_MASK_MOUSEMOVE       OI_EVENT_MASK(OI_MOUSEMOVE)
#define OI_MASK_MOUSEBUTTONUP   OI_EVENT_MASK(OI_MOUSEBUTTONUP)
#define OI_MASK_MOUSEBUTTONDOWN OI_EVENT_MASK(OI_MOUSEBUTTONDOWN)
#define OI_MASK_MOUSE           (OI_EVENT_MASK(OI_MOUSEMOVE) | \
				   OI_EVENT_MASK(OI_MOUSEBUTTONUP) | \
				   OI_EVENT_MASK(OI_MOUSEBUTTONDOWN))
#define OI_MASK_ACTIVE          OI_EVENT_MASK(OI_ACTIVE)
#define OI_MASK_RESIZE          OI_EVENT_MASK(OI_RESIZE)
#define OI_MASK_EXPOSE          OI_EVENT_MASK(OI_EXPOSE)
#define OI_MASK_WINDOW          (OI_EVENT_MASK(OI_ACTIVE) | \
				   OI_EVENT_MASK(OI_RESIZE) | \
				   OI_EVENT_MASK(EXPOSE))
#define OI_MASK_DISCOVERY       OI_EVENT_MASK(OI_DISCOVERY)
#define OI_MASK_ACTION          OI_EVENT_MASK(OI_ACTION)
#define OI_MASK_QUIT            OI_EVENT_MASK(OI_QUIT)

/* ********************************************************************
 * Special event structures
 ******************************************************************** */

// Device discovery event
typedef struct oi_discovery_event {
  uchar type;               // OI_DISCOVERY
  uchar device;             // Device index
  char *name;               // Short name
  char *description;        // Long description
  uint provides;            // Provide mask
} oi_discovery_event;

// Application visibility event
typedef struct oi_active_event {
  uchar type;               // OI_ACTIVE
  uchar device;             // Device index
  uchar gain;               // Focus was 0:lost 1:gained
  uint state;               // Mask of focus state
} oi_active_event;

// Keyboard event
typedef struct oi_keyboard_event {
  uchar type;               // OI_KEYUP or OI_KEYDOWN
  uchar device;             // Device index
  uchar state;              // Key is 0:up 1:down
  oi_keysym keysym;       // Key symbol
} oi_keyboard_event;

// Mouse move event
typedef struct oi_mousemove_event {
  uchar type;               // OI_MOUSEMOVE
  uchar device;             // Device index
  uchar state;              // Button states
  ushort x;                 // Absolute x coordinate
  ushort y;                 // Absolute y coordinate
  sshort relx;              // Relative x movement
  sshort rely;              // Relative y movement
} oi_mousemove_event;

// Mouse button event
typedef struct oi_mousebutton_event {
  uchar type;               // OI_MOUSEBUTTONUP or OI_MOUSEBUTTONDOWN
  uchar device;             // Device index
  uchar button;             // Mouse button index
  uchar state;              // Button states
  ushort x;                 // Absolute x coordinate at event time
  ushort y;                 // Absolute y coordinate at event time
} oi_mousebutton_event;

// Application window resize event
typedef struct oi_resize_event {
  uchar type;               // OI_RESIZE
  uchar device;             // Device index
  uint width;               // New window width
  uint height;              // New window height
} oi_resize_event;

// Application window needs redraw
typedef struct oi_expose_event {
  uchar type;               // OI_EXPOSE
} oi_expose_event;

// Quit event
typedef struct oi_quit_event {
  uchar type;               // OI_QUIT
} oi_quit_event;

// Action event
typedef struct oi_action_event {
  uchar type;               // OI_ACTION
  uchar device;             // Device index
  uint actionid;            // User-defined actionid
  uchar state;              // State (pressed/released)
  sint data1;               // Default data slot   (1d device: x coord)
  sint data2;               // Secondary data slot (2d device: y coord)
  sint data3;               // Tertiary data slot  (3d device: z coord)
} oi_action_event;

/* ********************************************************************
 * The generic (united) event structure
 ******************************************************************** */

// The united event structure
typedef union {
  uchar type;
  oi_active_event active;
  oi_keyboard_event key;
  oi_mousemove_event move;
  oi_mousebutton_event button;
  oi_resize_event resize;
  oi_expose_event expose;
  oi_quit_event quit;
  oi_discovery_event discover;
  oi_action_event action;
} oi_event;

/* ******************************************************************** */

#endif
