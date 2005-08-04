/*
 * events.c : The event handling functions
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

// Includes
#include "config.h"
#include <stdio.h>
#include <unistd.h>
#include "openinput.h"
#include "internal.h"

// Globals
static uint event_mask = 0;

/* ******************************************************************** */

// Look at events in queue without removal (public)
sint oi_events_peep(oi_event *evts, sint num) {
  sint p;

  queue_lock();
  p = queue_peep(evts, num, ~event_mask, FALSE);
  queue_unlock();

  return p;
}

/* ******************************************************************** */

// Add events to the queue (public)
sint oi_events_add(oi_event *evts, sint num) {
  int i;
  int j;
  int tot;

  queue_lock();

  // All add
  tot = 0;
  for(i=0; i<num; i++) {

    // If filter mask does not match, look at next
    if(!(OI_EVENT_MASK(evts[i].type) & ~event_mask)) {
      continue;
    }

    // Add it
    j = queue_add(&(evts[i]));
    tot += j;

    // As long as we don't overflow
    if(j <= 0) {
      break;
    }
  }

  queue_unlock();

  return tot;
}

/* ******************************************************************** */

// Explictly pump events into the queue (public)
void oi_events_pump() {
  static uint last = 0;
  uint now;

  // Bail out if 'no time' has passed
  now = oi_getticks();
  if(now == last) {
    return;
  }
  last = now;

  queue_lock();

  // Pump devices
  device_pumpall();

  // Handle keyboard repeats
  keyboard_dorepeat();
  
  queue_unlock();  
}

/* ******************************************************************** */

// Poll for an event (public)
sint oi_events_poll(oi_event *evt) {
  int found;

  oi_events_pump();

  // Peep for 1 event with removal
  found = queue_peep(evt, 1, ~event_mask, TRUE);

  return found;
}

/* ******************************************************************** */

// Wait for an event (public)
void oi_events_wait(oi_event *evt) {
  int found;

  // Wait until an event occurs
  found = 0;
  while(!found) {
    // Pump and read
    oi_events_pump();

    found = queue_peep(evt, 1, ~event_mask, TRUE);
    usleep(OI_SLEEP);
  }
}

/* ******************************************************************** */

// Set mask for incoming events (public)
void oi_events_setmask(uint mask) {
  oi_event ev;

  // Set mask and discard all pending events
  event_mask = mask;
  while(oi_events_poll(&ev)) {
    ;
  }
}

/* ******************************************************************** */

// Set mask for incoming events (public)
uint oi_events_getmask() {
  return event_mask;
}

/* ******************************************************************** */
