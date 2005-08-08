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

/**
 * @ingroup PMain
 * @brief Initialize OpenInput
 *
 * @param window_id window hook parameters, see @ref PWindow
 * @param flags library initialization flags, see @ref PFlags
 * @returns errorcode, see @ref PErrors
 *
 * This function prepares OpenInput for use, and must be called
 * before the library can be used.
 *
 * When invoked, the following happens:
 * -# the event queue is initialized
 * -# all devices are bootstrapped
 * -# available devices are initialized
 * -# the application state manager is initialized
 * -# the mouse state manager is initialized
 * -# the keyboard state manager is initialized
 * -# the action state manager is initialized
 * -# OpenInput enters "initialized mode"
 * -# you're good to go! ;-)
 */
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

/**
 * @ingroup PMain
 * @brief Shutdown the library
 *
 * @returns errorcode, see @ref PErrors
 *
 * Call this function to terminate OpenInput. You should call
 * this function to make OpenInput tidy up when your applicaiton
 * closes - it not, you may experience memory leaks and the like.
 */
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

/**
 * @ingroup IMain
 * @brief Get library run-state
 *
 * @returns run state (true or false)
 *
 * You can use this function to check if you are
 * allowed to generate events and otherwise use functions
 * in the library. It is especially important for device
 * drivers to check this, since events (apart from direct
 * queue injects) may not be generated when the library
 * is initializing.
 *
 * See oi_init for more information.
 */
inline sint oi_runstate() {
  return oi_running;
}

/* ******************************************************************** */

/**
 * @ingroup IMain
 * @brief Get timestamp
 *
 * @returns system ticks in ms
 *
 * This function returns the number of system ticks
 * with a resolution of 1/1000 second (ms). This can
 * be used for timestamps and alike.
 */
inline uint oi_getticks() {
  struct timeval now;
  uint ticks;

  // We do this the POSIX way
  gettimeofday(&now, NULL);
  ticks = now.tv_sec*1000 + now.tv_usec/1000;
  return ticks;
}

/* ******************************************************************** */
