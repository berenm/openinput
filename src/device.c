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
#include <stdio.h>
#include <string.h>
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

// Bootstrap devices, ie. fill array with available devices (internal)
void device_boot() {
  int i;
  int j;

  debug("device_boot");

  // Clear device array
  memset(devices, 0, sizeof(devices));
  
  // Fill structure array with available devices
  j = 0;
  for(i=0; bootstrap[i]; i++) {
    if(bootstrap[i]->avail()) {
      // Create the device and set basics
      devices[j] = bootstrap[i]->create();
      devices[j]->index = j;
      devices[j]->status = SINP_STA_DEAD;
      devices[j]->name = bootstrap[i]->name;
      devices[j]->desc = bootstrap[i]->desc;
      devices[j]->provides = bootstrap[i]->provides;
      j++;

      debug("device_boot: '%s' as index %i booted", devices[j]->name, j);
    }
  }
  num_devices = j;
  
  debug("device_boot: %i devices compiled in and %i available", i, j);
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
    devices[i]->process();
  }
}

/* ******************************************************************** */

// Initialize a device (public)
sint sinp_dev_init(sint index, char *window_id, uint flags) {
  sinp_device *dev;

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  return dev->init(window_id, flags); 
}

/* ******************************************************************** */

// Destroy a device (public)
sint sinp_dev_destroy(sint index) {
  sinp_device *dev;

  // Dummy check
  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // Check that the device is up
  if(dev->status <= SINP_STA_DEAD) {
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

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // We have to be alive to do this
  if(dev->status <= SINP_STA_DEAD) {
    return SINP_ERR_DEV_DEAD;
  }

  return dev->enable(on);
}

/* ******************************************************************** */

// Grab and lock input-provide-mask for this device if possible (public)
sint sinp_dev_grab(sint index, uint mask) {
  sinp_device *dev;

  dev = device_get(index);
  if(dev == NULL) {
    return SINP_ERR_INDEX;
  }

  // We have to be alive to do this
  if(dev->status <= SINP_STA_DEAD) {
    return SINP_ERR_DEV_DEAD;
  }

  return dev->grab(mask);
}

/* ******************************************************************** */

// Pump events from device into queue (public)
void sinp_dev_pump(sint index) {
  sinp_device *dev;
  
  // Make sure we exist
  dev = device_get(index);
  if(dev == NULL) {
    return;
  }

  // We have to be alive to do this
  if(dev->status <= SINP_STA_DEAD) {
    return;
  }

  dev->process();
}

/* ******************************************************************** */
