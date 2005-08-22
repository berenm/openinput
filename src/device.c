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
static uchar devices_run[OI_MAX_DEVICES];
static uint num_devices;
static sint more_avail;

// Include the bootstrap table
#define _DEVICE_FILLER_
#include "bootstrap.h"
#undef _DEVICE_FILLER_

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Initialize device manager
 *
 * @returns errorcode, see @ref PErrors
 *
 * Called on library initialization. This functions prepares the
 * device manager before devices are booted and initialized.
 */
sint device_init() {
  uint i;

  for(i=0; i<OI_MAX_DEVICES; i++) {
    devices[i] = NULL;
    devices_run[i] = FALSE;
  }
  num_devices = 0;
  more_avail = FALSE;

  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Bootstrap all devices
 *
 * @param window_id window hook parameters, see @ref PWindow
 * @param flags library initization flags
 *
 * This function parses the bootstrap table and
 * registers (bootstraps and initializes) all devices.
 */
void device_bootstrap(char *window_id, uint flags) {
  uint i;
  uint j;
  sint ok;

  debug("device_bootstrap");

  // Critical error - bootstrap is empty
  if(!bootstrap || !bootstrap[0]) {
    return;
  }
  if(num_devices != 0) {
    debug("device_bootstrap: devices already initialized");
    return;
  }

  // Fill structure array with available devices
  j = 0;
  for(i=0; bootstrap[i]; i++) {
    debug("device_bootstrap: checking bootstrap entry %u", i, bootstrap[i]->name);
    
    // Some drivers may control more devices, allow them using the more_avail
    more_avail = FALSE;
    do {
      // Be pessimistic
      ok = FALSE;

      // Check bootstrap entry
      if(bootstrap[i] &&
	 bootstrap[i]->avail && bootstrap[i]->create &&
	 bootstrap[i]->name && bootstrap[i]->desc) {

	// Check that the device is available
	if(bootstrap[i]->avail(flags)) {
	  
	  // Register device, but continue even if that fails
	  ok = TRUE;
	  if(device_register(bootstrap[i], window_id, flags) == OI_ERR_OK) {
	    j++;
	  }
	}
      }
    
      // Continue till register fails or driver exhausted
    } while(ok && more_avail);
  }
  
  debug("device_bootstrap: %u drivers compiled in, %u devices analyzed" \
	" and %u available", i, j, num_devices);
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Register new device via bootstrap
 *
 * @param boot pointer to bootstrap structure
 * @param window_id window hook parameters, see @ref PWindow
 * @param flags library initialization flags, see @ref PFlags
 * @returns errorcode, see @ref PErrors
 *
 * @pre bootstrap structure pointed to by "boot" must be valid
 *
 * Helper function the initialize a device driver. This
 * is automatically called for all "available" devices on
 * library initilization
 */
sint device_register(oi_bootstrap *boot, char *window_id, uint flags) {
  oi_event ev;

  // Create the device
  devices[num_devices] = boot->create();
  if(devices[num_devices] == NULL) {
    return OI_ERR_NO_DEVICE;
  }

  // Check required functions
  if(!devices[num_devices]->init ||
     !devices[num_devices]->destroy ||
     !devices[num_devices]->process) {
    
    // Device creation failed, abort
    return OI_ERR_NOT_IMPLEM;
  }
  
  // Fill trivial stuff - must happen before the init()
  devices[num_devices]->index = num_devices+1;
  devices[num_devices]->name = boot->name;
  devices[num_devices]->desc = boot->desc;
  devices[num_devices]->provides = boot->provides;
  
  // Ok, initialize the device
  if(devices[num_devices]->init(devices[num_devices], window_id, flags) != OI_ERR_OK) {

    // Init failed, free the device structure and abort
    devices[num_devices]->destroy(devices[num_devices]);
    return OI_ERR_NO_DEVICE;
  }
  
  // Send the discovery event
  ev.type = OI_DISCOVERY;
  ev.discover.device = num_devices+1;
  ev.discover.name = boot->name;
  ev.discover.description = boot->desc;
  ev.discover.provides = boot->provides;     
  queue_add(&ev);
  
  // Enable device for event pumping
  devices_run[num_devices] = TRUE;
  debug("device_bootstrap: device '%s' (%s) added at index %i",
	devices[num_devices]->name, devices[num_devices]->desc, num_devices+1);
  
  // Ok, we're done
  num_devices++;
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
 * @brief Driver can initialize more devices
 *
 * @param more true (1) for more available, false (0) otherwise
 *
 * Tell the device initialization framework, that the device driver
 * who's "bootstrap->create" or "bootstrap->avail" function is
 * currently run can (possibly) initialize more drivers. This could be
 * the case if a system has two or more joysticks connected.
 *
 * You do not have to use this control - it is assumed that drivers
 * control only a single device. If however, your driver can control
 * more devices, call this function with the "more" param set to true.
 * The device driver framework will then do another pass at the
 * avail/create invokation. This can be repeated as long as the
 * device table is not full.
 */
void device_moreavail(sint more) {
  if(more) {
    more_avail = TRUE;
  }
  else {
    more_avail = FALSE;
  }
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
 * Run through all devices and process them, ie.
 * make them pump events into the queue.
 */
inline void device_pumpall() {
  uint i;

  for(i=0; i<num_devices; i++) {
    // Only pump devices if it's there and enabled!
    if(devices[i] && (devices_run[i] == TRUE)) {
      devices[i]->process(devices[i]);
    }
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

/**
 * @ingroup PDevice
 * @brief Enable/disable a device
 *
 * @param index index of device to enable/disable
 * @param q new state of device or query, see @ref PBool
 * @returns state of device, OI_QUERY on error
 *
 * Use this function to turn a device on or off. Devices that are
 * off do not generate events and do not update the state managers.
 * All devices defaults to "on".
 */
oi_bool oi_device_enable(uchar index, oi_bool q) {
  uchar enable;

  // Check that the devices exists
  if(device_get(index) == NULL) {
    return OI_QUERY;
  }
  
  switch(q) {
  case OI_ENABLE:
    enable = TRUE;
    break;

  case OI_DISABLE:
    enable = FALSE;
    break;

  case OI_QUERY:
    if(devices_run[index-1]) {
      return OI_ENABLE;
    }
    else {
      return OI_DISABLE;
    }
  }
  
  // Set new device state
  devices_run[index-1] = enable;
  return q;
}

/* ******************************************************************** */

/**
 * @ingroup PDevice
 * @brief Get information about a device
 *
 * @param index index of device to get info about
 * @param name pointer to string with device name
 * @param desc pointer to string with device description
 * @param provides pointer to provide flags, see @ref PProvide
 * @returns errorcode, see @ref PErrors
 *
 * Use this function to obtain some basic information about
 * an initialized device driver. The device index (parameter "dev")
 * can be obtained from most event structures, and in particular
 * via the discovery events.
 *
 * @note
 * The pointers you receive may NOT be freed, as
 * they are internal OpenInput library data!
 */
sint oi_device_info(uchar index, char **name, char **desc, uint *provides) {
  oi_device *dev;

  // Get device
  dev = device_get(index);
  if(dev == NULL) {
    return OI_ERR_NO_DEVICE;
  }

  // Ok, fill
  if(name) {
    *name = dev->name;
  }
  if(desc) {
    *desc = dev->desc;
  }
  if(provides) {
    *provides = dev->provides;
  }
  return OI_ERR_OK;
}

/* ******************************************************************** */
