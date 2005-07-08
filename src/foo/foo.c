/*
 * foo.c : Foo (test) utility functions (bootstrapping, etc.)
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
#include "sinp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "bootstrap.h"
#include "foo.h"

// Bootstrap global
sinp_bootstrap foo_bootstrap = {
  "foo",
  "Foo test system",
  SINP_PRO_UNKNOWN,
  foo_avail,
  foo_device
};

// Globals
static sinp_device *dev = NULL;
static uint grabmask = 0;

/* ******************************************************************** */
 
// Check availablity of foo
sint foo_avail() {
  debug("foo_avail");

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Install foo 'device' block
sinp_device *foo_device() {
  debug("foo_device");

  // Dummy check
  if(dev != NULL) {
    debug("foo_device: device struct already exists");
    return NULL;
  }

  // Alloc
  dev = (sinp_device*)malloc(sizeof(sinp_device));
  if(dev == NULL) {
    debug("foo_device: device creation failed");
    return NULL;
  }

  // Set members
  memset(dev, 0, sizeof(sinp_device));
  dev->init = foo_init;
  dev->enable = foo_enable;
  dev->destroy = foo_destroy;
  dev->process = foo_process;  
  dev->grab = foo_grab;
  
  // Done
  return dev;
}

/* ******************************************************************** */

// Initialize foo
sint foo_init(char *window_id, uint flags) {
  debug("foo_init: window '%s', flags %i", window_id, flags);

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Enable foo driver
sint foo_enable(sint on) {
  debug("foo_enable");
  
  // Status is now online
  if(dev) {
    if(on) {
      dev->status |= SINP_STA_ACTIVE;
    }
    else {
      dev->status = dev->status - (dev->status & SINP_STA_ACTIVE);
    }
  }

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Free foo device structure
sint foo_destroy(sinp_device *dev) {
  debug("foo_destroy");

  if(dev) {
    free(dev);
    dev = NULL;
  }

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Pump events into event queue
void foo_process() {
  debug("foo_process");
}

/* ******************************************************************** */

// Pump events into event queue
sint foo_grab(uint mask) {
  debug("foo_grab");

  // We only implement SINP_PRO_UNKNOWN
  if((mask != 0) || (mask != SINP_PRO_UNKNOWN)) {
    return SINP_ERR_PARAM;
  }

  // Toggle
  if(mask) {
    grabmask |= mask;
  }
  else {
    grabmask = grabmask - (grabmask & mask);
  }

  return SINP_ERR_OK;
}

/* ******************************************************************** */
