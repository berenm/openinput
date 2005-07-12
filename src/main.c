/*
 * main.c : The initialization and shutdown functions
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

// Includes
#include "config.h"
#include <stdio.h>
#include "sinp.h"
#include "internal.h"

/* ******************************************************************** */

// Initialize default devices
sint sinp_init(char *window_id, uint flags) {
  int i;
  int e;

  debug("sinp_init");

  // Bootstrap all devices
  device_bootstrap();

  // Parse all devices
  i = 0;
  e = 0;
  while(device_get(i) != NULL) {

    // Initialize device
    if(device_init(i, window_id, flags) != SINP_ERR_OK) {
      // Error initializing, count
      e++;
    }

    i++;
  }
  
  return e;
}

/* ******************************************************************** */

// Shutdown sinp
sint sinp_close() {
  int i;
  int e;

  debug("sinp_close");

  // Parse all devices
  i = 0;
  e = 0;
  while(device_get(i) != NULL) {

    // Destroy it
    if(device_destroy(i) != SINP_ERR_OK) {
      e++;
    }

    i++;
  }

  // Done
  return e;
}

/* ******************************************************************** */
