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
#include <time.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "openinput.h"
#include "internal.h"

// Globals
static uint event_mask = 0;

/* ******************************************************************** */

/**
 * @ingroup PEvents
 * @brief Take a peep at the queue
 *
 * @param evts where to place the event(s)
 * @param num number of events to return
 * @returns number of events returned
 *
 * Take a peep at the event queue. That is, look at
 * the first events in the queue but do remove the
 * events from the queue
 */
sint oi_events_peep(oi_event *evts, sint num) {
  sint p;

  queue_lock();
  p = queue_peep(evts, num, ~event_mask, FALSE);
  queue_unlock();

  return p;
}

/* ******************************************************************** */

/**
 * @ingroup PEvents
 * @brief Add events to the queue
 *
 * @param evts pointer to event(s)
 * @param num number of events to add
 * @returns number of events added
 *
 * Inject events to the event queue
 */
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

/**
 * @ingroup PEvents
 * @brief Pump events into the queue
 *
 * Explicitly make all device-drivers pump events into the
 * event queue. You should probably NOT use this function
 * as both oi_events_poll and oi_events_wait calls the
 * pump-function automatically
 *
 * What happens in this function is:
 * -# Make sure time has elapsed since last call
 * -# Lock the queue
 * -# Clear analogue action manager states
 * -# Pump all devices
 * -# Handle repeating keyboard events
 * -# Re-pump joystick manager to generate collected events
 * -# Unlock queue
 */
void oi_events_pump() {
  static uint last = 0;
  uint now;

  // Bail out if 'no time' has passed
  now = oi_getticks();
  if(now == last) {
    return;
  }
  last = now;

  // The very essence of OpenInput is the following lines
  queue_lock();

  action_clearreal();
  device_pumpall();
  keyboard_dorepeat();  
  joystick_pump();

  queue_unlock();  
}

/* ******************************************************************** */

/**
 * @ingroup PEvents
 * @brief Poll for events
 *
 * @param evt pointer of where to store event
 * @returns true (1) when events are available, false (0) otherwise
 *
 * Poll the event queue for events. This is typically the function
 * you want to use in a game, as the function returns immediately.
 * Use the function inside a while()-loop to handle all pending
 * events
 */
sint oi_events_poll(oi_event *evt) {
  int found;

  oi_events_pump();

  // Peep for 1 event with removal
  found = queue_peep(evt, 1, ~event_mask, TRUE);

  return found;
}

/* ******************************************************************** */

/**
 * @ingroup PEvents
 * @brief Wait for events
 *
 * @param evt pointer of where to store event
 *
 * For for a single event to appear. This call is blocking, so
 * you probably only want to use this if you're writing a
 * regular application (ie. not a game with high FPS requirements)
 */
void oi_events_wait(oi_event *evt) {
  int found;

  // Wait until an event occurs
  found = 0;
  while(!found) {
    // Pump and read
    oi_events_pump();

    found = queue_peep(evt, 1, ~event_mask, TRUE);
    
#ifdef HAVE_NANOSLEEP
    {
      // Use nanosleep under POSIX
      struct timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = OI_SLEEP * 1000000;
      nanosleep(&ts, NULL);
    }
#elif WIN32
    Sleep(OI_SLEEP);
#endif
  }
}

/* ******************************************************************** */

/**
 * @ingroup PEvents
 * @brief Set event filter mask
 *
 * @param mask event mask
 *
 * Filter out events using a event mask. See
 * @ref PEventmask for mask definitions.
 * A mask of 0 (zero) will not filter any events, which
 * is the default.
 */
void oi_events_setmask(uint mask) {
  oi_event ev;

  // Set mask and discard all pending events
  event_mask = mask;
  while(oi_events_poll(&ev)) {
    ;
  }
}

/* ******************************************************************** */

/**
 * @ingroup PEvents
 * @brief Get event filter mask
 *
 * @returns current event mask
 *
 * Return the current event filter mask, see
 * also @ref PEventmask
 */
uint oi_events_getmask() {
  return event_mask;
}

/* ******************************************************************** */
