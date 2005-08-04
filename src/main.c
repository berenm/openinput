/*
 * main.c : The initialization and shutdown functions
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
#include <time.h>
#include <sys/time.h>
#include "openinput.h"
#include "internal.h"

// Globals
static sint oi_running;

/* ******************************************************************** */

// Initialize default devices (public)
sint oi_init(char *window_id, uint flags) {
  int i;
  int e;

  debug("oi_init");

  // Initialize queue
  oi_running = FALSE;
  queue_init();

  // Bootstrap all devices
  device_bootstrap(flags);

  // Parse all devices
  e = 0;
  i = 1;
  while(device_get(i) != NULL) {

    // Initialize device
    if(device_init(i, window_id, flags) != OI_ERR_OK) {
      // Error initializing, count
      e++;
    }

    i++;
  }

  // Initialize state managers
  appstate_init();
  mouse_init();
  keyboard_init();
  action_init();

  // Set running flag
  oi_running = TRUE;
    
  return e;
}

/* ******************************************************************** */

// Shutdown the library (public)
sint oi_close() {
  int i;
  int e;

  debug("oi_close");
  oi_running = FALSE;

  // Parse all devices
  e = 0;
  i = 1;
  while(device_get(i) != NULL) {

    // Destroy it
    if(device_destroy(i) != OI_ERR_OK) {
      e++;
    }

    i++;
  }

  // Done
  return e;
}

/* ******************************************************************** */

// Return library running state (internal)
inline sint oi_runstate() {
  return oi_running;
}

/* ******************************************************************** */

// Return current system ticks (internal)
inline uint oi_getticks() {
  struct timeval now;
  uint ticks;

  // We do this the POSIX way
  gettimeofday(&now, NULL);
  ticks = now.tv_sec*1000 + now.tv_usec/1000;
  return ticks;
}

/* ******************************************************************** */
