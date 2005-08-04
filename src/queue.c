/*
 * queue.c : The event queue functions
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
#include <string.h>
#include "openinput.h"
#include "internal.h"

// Globals
static struct {
  oi_event events[OI_MAX_EVENTS];
  sshort head;
  sshort tail;
} queue;

/* ******************************************************************** */

// Initialize queue-event system (internal)
sint queue_init() {
  debug("queue_init");
  
  // Clear event queue
  queue.head = 0;
  queue.tail = 0;
  memset(queue.events, 0, sizeof(queue.events));

  //FIXME: Mutexes and threads should gracefully be started here

  // All done
  return OI_ERR_OK;
}

/* ******************************************************************** */

// Lock queue (internal)
inline sint queue_lock() {
  //FIXME: Implement this
  return OI_ERR_NOT_IMPLEM;
}

/* ******************************************************************** */

// Unlock queue (internal)
inline sint queue_unlock() {
  //FIXME: Implement this
  return OI_ERR_NOT_IMPLEM;
}

/* ******************************************************************** */

// Add events to queue (internal)
sint queue_add(oi_event *evt) {
  int tail, add;

  //FIXME Generate action events on keyboard/mouse
  if((evt->type == OI_KEYUP) ||
     (evt->type == OI_KEYDOWN) ||
     (evt->type == OI_MOUSEMOVE) ||
     (evt->type == OI_MOUSEBUTTONUP) ||
     (evt->type == OI_MOUSEBUTTONDOWN)) {
    action_process(evt);
  }

  //FIXME: Check mask before we add the event

  // Find position for insertion
  tail = (queue.tail+1)%OI_MAX_EVENTS;

  // Overflow, drop it
  if(tail == queue.head) {
    add = 0;
  }
  
  // Insert it by COPYING!
  else {
    queue.events[queue.tail] = *evt;
    add = 1;
    // SDL does some special windowmanager event handling here

    debug("queue_add: type %i added at position %i",
	  evt->type, queue.tail);

    queue.tail = tail;
  }

  return add;
}

/* ******************************************************************** */

// Cut out an event from the queue (internal)
sint queue_cut(ushort where) {

  // Cut head
  if(where == queue.head) {
    // Simply increase head
    queue.head = (queue.head+1)%OI_MAX_EVENTS;
    return queue.head;
  }
  
  // Cut tail
  if(((where+1)%OI_MAX_EVENTS) == queue.tail) {
    queue.tail = where;
    return queue.tail;
  }

  // Cut somewhere in between
  else {
    sint next;
    sint here;

    // Wrap around negative tail
    --queue.tail;
    if(queue.tail < 0) {
      queue.tail = OI_MAX_EVENTS-1;
    }

    // Shift everything backwards
    for(here=where; here!=queue.tail; here=next) {
      next = (here+1)%OI_MAX_EVENTS;

      // We use COPYING here
      queue.events[here] = queue.events[next];
    }

    // Done
    return where;
  }
}

/* ******************************************************************** */

// Take a peep at the queue (internal)
sint queue_peep(oi_event *evts, sint num, uint mask, sint remove) {
  oi_event tmpevt;
  int here;
  int copy;

  // User wants to know if events for the mask are pending
  if((evts == NULL) || (num <= 0)) {
    evts = &tmpevt;
    num = 1;
    remove = 0;
  }

  // Start from head, continue till tail or num reached
  here = queue.head;
  copy = 0;
  while((copy < num) && (here != queue.tail)) {
    // Check mask
    if(mask & OI_EVENT_MASK(queue.events[here].type)) {

      // Transfer to user by COPYING
      evts[copy] = queue.events[here];
      copy++;

      // With or without removal
      if(remove) {
	here = queue_cut(here);
      }
      else {
	here = (here+1)%OI_MAX_EVENTS;
      }
    }

    // Mask does not match, fetch next event in queue
    else {
      here = (here+1)%OI_MAX_EVENTS;
    }
  }
  
  return copy;
}

/* ******************************************************************** */
