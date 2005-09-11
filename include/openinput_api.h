/*
 * openinput_api.h : Main API
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

#ifndef _OPENINPUT_API_H_
#define _OPENINPUT_API_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ******************************************************************** */

// Default initialization of all available devices (num_failed)
sint oi_init(char* window_id, uint flags);

// Shutdown all available devices (num_failed)
sint oi_close();

/* ******************************************************************** */

// Get device information (errorcode)
sint oi_device_info(uchar index, char **name, char **desc, uint *provides);

// Enable/disable device event pumping (state)
oi_bool oi_device_enable(uchar index, oi_bool q);

/* ******************************************************************** */

// Look at event without removing it from queue (number_returned)
sint oi_events_peep(oi_event *evts, sint num);

// Add an event to the tail of the queue (number_added)
sint oi_events_add(oi_event *evts, sint num);

// Pump all device to transfer events into queue (n/a)
void oi_events_pump();

// Poll events (more_pending)
sint oi_events_poll(oi_event *evt);

// Wait for an event (n/a)
void oi_events_wait(oi_event *evt);

// Set event type filter mask (n/a)
void oi_events_setmask(uint mask);

// Get event type filter mask (event_mask)
uint oi_events_getmask();

/* ******************************************************************** */

// Send events for down-state keys (errorcode)
sint oi_key_repeat(sint delay, sint interval);

// Get key state table and set num to number of elements (pointer)
uchar *oi_key_keystate(uchar index, sint *num);

// Return modifier mask (modifier_mask)
uint oi_key_modstate(uchar index);

// Get name of key (string)
char *oi_key_getname(oi_key key);

// Get key code given name (oi_key)
oi_key oi_key_getcode(char *name);

/* ******************************************************************** */

// Get absolute position of mouse (button_mask)
sint oi_mouse_absolute(uchar index, sint *x, sint *y);

// Get relative motion of mouse (button_mask)
sint oi_mouse_relative(uchar index, sint *x, sint *y);

// Warp mouse cursor position (errorcode)
sint oi_mouse_warp(uchar index, sint x, sint y);

// Get name of mouse button (string)
char *oi_mouse_getname(oi_mouse button);

// Get mouse-id given name (oi_mouse)
oi_mouse oi_mouse_getcode(char *name);

/* ******************************************************************** */

// Get absolute position of joystick axis (button_mask)
uint oi_joy_absolute(uchar index, uchar axis, sint *value, sint *second);

// Get relative motion of joystick axis (button_mask)
uint oi_joy_absolute(uchar index, uchar axis, sint *value, sint *second);

// Get name of joystick button/axis (string)
char *oi_joy_getname(uint code);

// Get mouse-id given name (oi_mouse)
uint oi_joy_getcode(char *name);

// Get basic information about a joystick device (errorcode)
sint oi_joy_info(uchar index, char **name, uchar *buttons, uchar *axes);

// Get advanced axes information for a joystick device (errorcode)
sint op_joy_axessetup(uchar index, oi_joytype *type[], uchar *pair[], uchar *num);

/* ******************************************************************** */

// Get focus state of application (focus_mask)
sint oi_app_focus();

// Show/hide cursor (state)
oi_bool oi_app_cursor(oi_bool q);

// Grab/ungrab input (state)
oi_bool oi_app_grab(oi_bool q);

/* ******************************************************************** */

// Install actionmap (errorcode)
sint oi_action_install(oi_actionmap *map, sint num);

// Check/validate single actionmap structure (errorcode)
sint oi_action_validate(oi_actionmap *map);

// Get action state table and set num to number of elements (pointer)
uchar *oi_action_actionstate(sint *num);

/* ******************************************************************** */

#endif
