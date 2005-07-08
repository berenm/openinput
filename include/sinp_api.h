/*
 * sinp_api.h : Main API
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

/* ********************************************************************
 * Special notes:
 * Each function call is preceeded by a small explanation and
 * what return type one can expect (in the last paranthesis).
 * If a "(M)" is present, the function is for manual device handling,
 * and should be avoided under normal circumstances, eg. when using
 * sinp_init() which sets devices automatically.
 *
 ******************************************************************** */

#ifndef _SINP_API_H_
#define _SINP_API_H_

#ifndef _SINP_H_
#error Do not include this file directly - use sinp.h
#endif

#define main sinp_main

/* ********************************************************************
 * Main functions
 ******************************************************************** */

// Main function - called when library is loaded (errorcode)
sint sinp_main(int argc, char *argv[]);

// Default initialization of all available devices (errorcode)
sint sinp_init(char* window_id, uint flags);

// Shutdown library (errorcode)
sint sinp_close();


/* ********************************************************************
 * Generic low-level device function
 ******************************************************************** */

// Return read-only name of a device (string)
char *sinp_dev_name(sint index);

// Return read-only name of device (string)
char *sinp_dev_description(sint index);

// Return read-only provide bitmask of device (bitmask)
uint  sinp_dev_provides(sint index);

// Return read-only status bitmask of device (bitmask)
uint  sinp_dev_status(sint index);

// Device initialization function (M) (errorcode)
sint  sinp_dev_init(sint index, char *window_id, uint flags);

// Shutdown a device (M) (errorcode)
sint  sinp_dev_destroy(sint index);

// Make device listen for events (M) (errorcode)
sint  sinp_dev_enable(sint index, sint on);

// Grab/ungrab input via "provide bitmask" for this device (errorcode)
sint  sinp_dev_grab(sint index, uint mask);

// Make device pump events into queue (n/a)
void  sinp_dev_pump(sint index);


/* ********************************************************************
 * Event functions
 ******************************************************************** */

// Look at event without removing it from queue (number_returned)
sint  sinp_events_peep(sinp_event *evts, sint num);

// Add an event to the tail of the queue (number_added)
sint  sinp_events_add(sinp_event *evts, sint num);

// Pump all device to transfer events into queue (n/a)
void  sinp_events_pump();

// Poll events (more_pending)
sint  sinp_events_poll(sinp_event *evt);

// Wait for an event (n/a)
void  sinp_events_wait(sinp_event *evt);

// Set event type filter mask (n/a)
void  sinp_events_setmask(uint mask);

// Get event type filter mask (mask)
uint  sinp_events_getmask();


/* ********************************************************************
 * Keyboard functions
 ******************************************************************** */

void  sinp_key_keyrepeat();
void  sinp_key_getstate();
void  sinp_key_modstate();
void  sinp_key_getname();


/* ********************************************************************
 * Mouse functions
 ******************************************************************** */

void  sinp_mouse_getstate();
void  sinp_mouse_getrelative();
void  sinp_mouse_warp();
void  sinp_mouse_showcursor();

/* ********************************************************************
 * Misc functions
 ******************************************************************** */

void  sinp_misc_appstate();

/* ******************************************************************** */

#endif
