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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sinp.h"
#include "internal.h"

// Globals
static sinp_device *devices[SINP_MAX_DEVICES];
static sint num_devices = 0;

// Include the bootstrap table
#define _DEVICE_FILLER_
#include "bootstrap.h"
#undef _DEVICE_FILLER_

/* ******************************************************************** */

// Register device via bootstrap (internal)
sint device_register(sinp_bootstrap *boot) {
  // Create the device and set basics
  devices[num_devices] = boot->create();
  if(devices[num_devices] != NULL) {
    devices[num_devices]->index = num_devices;
    devices[num_devices]->status = SINP_STA_DEAD;
    devices[num_devices]->name = boot->name;
    devices[num_devices]->desc = boot->desc;
    devices[num_devices]->provides = boot->provides;
    num_devices++;
    return SINP_ERR_OK;
  }
  
  // Failure
  return SINP_ERR_NO_DEVICE;
}

/* ******************************************************************** */

// Initialize builtin devices (internal)
void device_boot() {
  int i;
  int j;

  debug("device_boot");

  if(num_devices != 0) {
    debug("device_boot: devices already initialized");
    return;
  }

  // Fill structure array with available devices
  j = 0;
  for(i=0; bootstrap[i]; i++) {
    debug("device_boot: checking bootstrap entry %i", i, bootstrap[i]->name);
    
    // If available, register
    if(bootstrap[i]->avail()) {
      device_register(bootstrap[i]);      

      debug("device_boot: device '%s' (%s) added at index %i",
	    devices[i]->name, devices[i]->desc, num_devices);
    }
  }
  
  debug("device_boot: %i devices compiled in and %i available",
	i, num_devices);
}

/* ******************************************************************** */

// Return a device structure (internal)
sinp_device *device_get(sint index) {
  // Dummy check
  if((index < 0) || (index > num_devices)) {
    debug("device_get: error, index %i does not exist", index);
    return NULL;
  }
  
  return devices[index];
}

/* ******************************************************************** */

// Set device provides-flag (internal)
void device_set_provides(sint index, uint pro) {
  sinp_device *dev;
  
  dev = device_get(index);
  if(dev == NULL) {
    return;
  }

  dev->provides = pro;
}

/* ******************************************************************** */

// Set device status-flag (internal)
void device_set_status(sint index, uint sta) {
  sinp_device *dev;

  dev = device_get(index);
  if(dev == NULL) {
    return;
  }

  dev->status = sta;
}

/* ******************************************************************** */

// Pump events from all devices into queue (internal)
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
  if(sub = index(str, tok)) {
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

// Initialize a device (public)
sint sinp_dev_init(sint index, char *window_id, uint flags) {
  sinp_device *dev;
  int e;

  debug("sinp_dev_init");

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // Graceful device creation
  e = dev->init(dev, window_id, flags);
  if(e != SINP_ERR_OK) {
    return e;
  }

  // Animate the dead and we're done
  dev->status = SINP_STA_ALIVE;
  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Destroy a device (public)
sint sinp_dev_destroy(sint index) {
  sinp_device *dev;

  debug("sinp_dev_destroy");

  // Dummy check
  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // Check that the device is up
  if(dev->status == SINP_STA_DEAD) {
    return SINP_ERR_DEV_DEAD;
  }

  // Kill device
  dev->status = SINP_STA_DEAD;
  return dev->destroy(dev);
}

/* ******************************************************************** */

// Return name of a device (public)
char *sinp_dev_name(sint index) {
  sinp_device *dev;

  debug("sinp_dev_name");

  dev = device_get(index);
  if(dev == NULL) {
    return NULL;
  }

  return dev->name;
}

/* ******************************************************************** */

// Return description of a device (public)
char *sinp_dev_description(sint index) {
  sinp_device *dev;

  debug("sinp_dev_description");

  dev = device_get(index);
  if(dev == NULL) {
    return NULL;
  }

  return dev->desc;
}

/* ******************************************************************** */

// Return status-flag of a device (public)
uint sinp_dev_status(sint index) {
  sinp_device *dev;

  debug("sinp_dev_status");

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  return dev->status;
}

/* ******************************************************************** */

// Return provides-flag of a device (public)
uint sinp_dev_provides(sint index) {
  sinp_device *dev;

  debug("sinp_dev_provides");

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  return dev->provides;
}

/* ******************************************************************** */

// Enable a device to generate events (public)
sint sinp_dev_enable(sint index, sint on) {
  sinp_device *dev;
  sint e;
  sint old; 

  debug("sinp_dev_enable");

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // Dummy checks
  if(dev->status == SINP_STA_DEAD) {
    debug("sinp_dev_enable: device is dead");
    return SINP_ERR_DEV_DEAD;
  }
  else if(on && (dev->status == SINP_STA_RUNNING)) {
    debug("sinp_dev_enable: device already running");
    return SINP_ERR_DEV_RUNNING;
  }
  else if(!on && (dev->status == SINP_STA_ALIVE)) {
    debug("sinp_dev_enable: device already alive");
    return SINP_ERR_DEV_DEAD;
  }

  // Toggle the bit
  old = dev->status;
  if(on) {
    dev->status = SINP_STA_RUNNING;
  }
  else {
    dev->status = SINP_STA_ALIVE;
  }

  // Notify device
  e = dev->enable(dev, on);

  // Fallback if we failed
  if(e != SINP_ERR_OK) {
    dev->status = old;
  }

  return e;
}

/* ******************************************************************** */

// Pump events from device into queue (public)
void sinp_dev_pump(sint index) {
  sinp_device *dev;

  debug("sinp_dev_pump");
  
  // Make sure we exist
  dev = device_get(index);
  if(dev == NULL) {
    return;
  }

  // We have to be alive to do this
  if(dev->status == SINP_STA_DEAD) {
    return;
  }

  dev->process(dev);
}

/* ******************************************************************** */

// Grab and lock input-provide-mask for this device if possible (public)
sint sinp_dev_grab(sint index, uint mask) {
  sinp_device *dev;

  debug("sinp_dev_grab");

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // We have to be alive to do this
  if(dev->status == SINP_STA_DEAD) {
    return SINP_ERR_DEV_DEAD;
  }

  return dev->grab(dev, mask);
}

/* ******************************************************************** */
