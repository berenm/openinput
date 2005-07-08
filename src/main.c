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
  int pro;
  int i;
  int j;

  debug("sinp_init");

  device_boot();

  // Parse all devices
  i = 0;
  pro = 0;
  while(device_get(i) != NULL) {

    // Can only raise the dead
    if(sinp_dev_status(i) == SINP_STA_DEAD) {
     
      // We only need one of each type (provider)
      if(!(sinp_dev_provides(i) & pro)) {

	// Initialize device
	if((j = sinp_dev_init(i, window_id, flags)) != SINP_ERR_OK) {
	  return j;
	}

	// Enable device
	sinp_dev_enable(i, 1);

	// Remember what it provides
	pro |= sinp_dev_provides(i);
      }
    }

    // Next
    i++;
  }

  // Initialize queue
  if((j = queue_init()) != SINP_ERR_OK) {
    return j;
  }

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Shutdown sinp
sint sinp_close() {
  int i;
  int j;

  debug("sinp_close");

  // Parse all devices
  i = 0;
  while(device_get(i) != NULL) {

    // Can only kill the living
    if(sinp_dev_status(i) != SINP_STA_DEAD) {

      debug("sinp_close: shutdown device %i", i);

      // Ungrab and disable
      sinp_dev_grab(i, 0);
      sinp_dev_enable(i, 0);

      // Destroy it
      if((j = sinp_dev_destroy(i)) != SINP_ERR_OK) {
	return j;
      }
    }
    else {
      debug("sinp_close: device %i already down", i);
    }

    // Next
    i++;
  }

  // Done
  return SINP_ERR_OK;
}

/* ******************************************************************** */
