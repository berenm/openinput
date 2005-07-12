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
  sshort xrel;              // Relative x movement
  sshort urel;              // Relative y movement
} sinp_mousemove_event;

// Mouse button event
typedef struct sinp_mousebutton_event {
  uchar type;               // SINP_MOUSEBUTTONUP or SINP_MOUSEBUTTONDOWN
  uchar device;             // Device index
  uchar button;             // Mouse button index
  uchar state;              // Button was 0:released 1:pressed
  ushort x;                 // Absolute x coordinate at event time
  ushort y;                 // Absolute y coordinate at event time
} sinp_mousebutton_event;

// Application window resize event
typedef struct sinp_resize_event {
  uchar type;               // SINP_RESIZE
  uint width;               // New window width
  uint height;              // New window height
} sinp_resize_event;

// Application window needs redraw
typedef struct sinp_expose_event {
  uchar type;               // SINP_EXPOSE
} sinp_expose_event;

// Quit event
typedef struct sinp_quit {
  uchar type;               // SINP_QUIT
} sinp_quit_event;

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
} sinp_event;

/* ******************************************************************** */

#endif
