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

/* ******************************************************************** */

#ifndef _SINP_API_H_
#define _SINP_API_H_

#ifndef _SINP_H_
#error Do not include this file directly - use sinp.h
#endif

/* ********************************************************************
 * Main functions
 ******************************************************************** */

// Default initialization of all available devices (num_failed)
sint sinp_init(char* window_id, uint flags);

// Shutdown all available devices (num_failed)
sint sinp_close();

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
