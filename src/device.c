/*
 * device.c : The generic device functions
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
#include "sinp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "internal.h"

// Globals
static sinp_device *devices[SINP_MAX_DEVICES];
static sint num_devices = 0;

// Include the bootstrap table
#define _DEVICE_FILLER_
#include "bootstrap.h"
#undef _DEVICE_FILLER_

/* ******************************************************************** */

// Register device via bootstrap
sint device_register(sinp_bootstrap *boot) {
  // Create the device and set basics
  devices[num_devices] = boot->create();
  if(devices[num_devices] != NULL) {

    // Fill trivial stuff
    devices[num_devices]->index = num_devices;
    devices[num_devices]->name = boot->name;
    devices[num_devices]->desc = boot->desc;
    devices[num_devices]->provides = boot->provides;

    debug("device_bootstrap: device '%s' (%s) added at index %i",
	  devices[num_devices]->name, devices[num_devices]->desc, num_devices);

    // Send the discovery event since we know enough already
    {
      sinp_event ev;
      ev.type = SINP_DISCOVERY;
      ev.discover.device = num_devices;
      ev.discover.name = boot->name;
      ev.discover.description = boot->desc;
      ev.discover.provides = boot->provides;     
      queue_add(&ev);
    }

    num_devices++;
    return SINP_ERR_OK;
  }
  
  // Failure
  return SINP_ERR_NO_DEVICE;
}

/* ******************************************************************** */

// Bootstrap devices using the bootstrap table
void device_bootstrap() {
  int i;
  int j;

  debug("device_bootstrap");

  if(num_devices != 0) {
    debug("device_bootstrap: devices already initialized");
    return;
  }

  // Fill structure array with available devices
  j = 0;
  for(i=0; bootstrap[i]; i++) {
    debug("device_bootstrap: checking bootstrap entry %i", i, bootstrap[i]->name);
    
    // If available, register
    if(bootstrap[i]->avail()) {
      device_register(bootstrap[i]);      
    }
  }
  
  debug("device_bootstrap: %i devices compiled in and %i available",
	i, num_devices);
}

/* ******************************************************************** */

// Initialize all devices which have been booted
sint device_init(sint index, char *window_id, uint flags) {
  sinp_device *dev;
  int e;

  debug("device_init");

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // Graceful device creation
  e = dev->init(dev, window_id, flags);
  if(e != SINP_ERR_OK) {
    return e;
  }

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Destroy a device
sint device_destroy(sint index) {
  sinp_device *dev;

  debug("device_destroy");

  // Dummy check
  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // Kill device
  return dev->destroy(dev);
}

/* ******************************************************************** */

// Return a device structure
sinp_device *device_get(sint index) {
  // Dummy check
  if((index < 0) || (index > num_devices)) {
    debug("device_get: error, index %i does not exist", index);
    return NULL;
  }
  
  return devices[index];
}

/* ******************************************************************** */

// Pump events from all devices into queue
void device_pumpall() {
  int i;

  for(i=0; i<num_devices; i++) {
    devices[i]->process(devices[i]);
  }
}

/* ******************************************************************** */

// Parse and convert window_id string into uints
uint device_windowid(char *str, char tok) {
  char *match;
  char *sub;
  int e;
  uint val;
  
  match = malloc(5);
  memset(match, 0, 5);

  // Set token to find, and scan
  match[0] = tok;
  strcat(match, ":%u");
  if((sub = index(str, tok))) {
    e = sscanf(sub, match, &val);
  }
  else {
    e = 0;
  }
  free(match);
  
  if(e != 1) {
    debug("device_windowid: parameter not found");
    return 0;
  }

  return val;
}

/* ******************************************************************** */
