/*
 * device.c : The generic device functions
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
#include "openinput.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "internal.h"

// Globals
static oi_device *devices[OI_MAX_DEVICES];
static int num_devices = 0;

// Include the bootstrap table
#define _DEVICE_FILLER_
#include "bootstrap.h"
#undef _DEVICE_FILLER_

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Register new device via bootstrap
 *
 * @param boot pointer to bootstrap structure
 * @returns errorcode, see @ref PErrors
 *
 * Helper function the initialize a device driver. This
 * is automatically called for all "available" devices on
 * library initilization
 */
sint device_register(oi_bootstrap *boot) {

  // Create the device and set basics
  devices[num_devices] = boot->create();
  if(devices[num_devices] != NULL) {

    // Check required functions
    if(!devices[num_devices]->init ||
       !devices[num_devices]->destroy ||
       !devices[num_devices]->process) {
      return OI_ERR_NOT_IMPLEM;
    }

    // Fill trivial stuff
    devices[num_devices]->index = num_devices+1;
    devices[num_devices]->name = boot->name;
    devices[num_devices]->desc = boot->desc;
    devices[num_devices]->provides = boot->provides;

    debug("device_bootstrap: device '%s' (%s) added at index %i",
	  devices[num_devices]->name, devices[num_devices]->desc, num_devices+1);

    // Send the discovery event
    {
      oi_event ev;
      ev.type = OI_DISCOVERY;
      ev.discover.device = num_devices;
      ev.discover.name = boot->name;
      ev.discover.description = boot->desc;
      ev.discover.provides = boot->provides;     
      queue_add(&ev);
    }

    num_devices++;
    return OI_ERR_OK;
  }
  
  // Failure
  return OI_ERR_NO_DEVICE;
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Bootstrap all devices
 *
 * @param flags library initization flags
 *
 * This function parses the bootstrap table, runs the
 * avail-function and possibly created the device
 * using device_register. The function is called on
 * library initialization.
 */
void device_bootstrap(uint flags) {
  uint i;
  uint j;

  debug("device_bootstrap");

  if(num_devices != 0) {
    debug("device_bootstrap: devices already initialized");
    return;
  }

  // Fill structure array with available devices
  j = 0;
  for(i=0; bootstrap[i]; i++) {
    debug("device_bootstrap: checking bootstrap entry %u", i, bootstrap[i]->name);
    
    // If available, register
    if(bootstrap[i] && bootstrap[i]->avail && bootstrap[i]->create &&
       bootstrap[i]->name && bootstrap[i]->desc) {
      if(bootstrap[i]->avail(flags)) {
	device_register(bootstrap[i]);      
      }
    }
  }
  
  debug("device_bootstrap: %u devices compiled in and %u available",
	i, num_devices);
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Initialize all devices
 *
 * @param index device index
 * @param window_id window init string, see @ref PWindow
 * @param flags init flags, see @ref PFlags
 * @returns errorcode, see @ref PErrors
 * 
 * This function initializes a device, ie. calls the
 * device structure "init" function.
 */
sint device_init(sint index, char *window_id, uint flags) {
  oi_device *dev;
  int e;

  debug("device_init");

  dev = device_get(index);
  if(dev == NULL) {
    return OI_ERR_INDEX;
  }

  // Graceful device creation
  e = dev->init(dev, window_id, flags);
  if(e != OI_ERR_OK) {
    return e;
  }

  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Destroy a device
 *
 * @param index device index
 * @returns errorcode, see @ref PErrors
 * 
 * Shutdown a device.
 */
sint device_destroy(sint index) {
  oi_device *dev;

  debug("device_destroy");

  // Dummy check
  dev = device_get(index);
  if(dev == NULL) {
    return OI_ERR_INDEX;
  }

  // Kill device
  return dev->destroy(dev);
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Get device structure
 *
 * @param index device index
 * @returns pointer to device structure
 * 
 * Fetch a device structure given device index.
 */
oi_device *device_get(sint index) {
  // Dummy check
  if((index < 1) || (index > num_devices)) {
    return NULL;
  }
  
  return devices[index-1];
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Pump events from all devices
 *
 * 
 * Run through all devices and process them, ie.
 * make them pump events into the queue.
 */
inline void device_pumpall() {
  uint i;

  for(i=0; i<num_devices; i++) {
    devices[i]->process(devices[i]);
  }
}

/* ******************************************************************** */


/**
 * @ingroup IDevice
 * @brief Parse window_id init string
 *
 * @param str pointer to window_id string, see @ref PWindow
 * @param tok parameter to parse, see @ref PWindow
 * @returns converted parameter (as an uint)
 *
 * Parse and convert string parameter to an uint. Basically
 * this is just an advanced atoi-function.
 */
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
