/*
 * linuxjoy.h: GNU/Linux joystick driver
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/joystick.h>
#include "internal.h"
#include "bootstrap.h"
#include "linuxjoy.h"
#include "linuxjoy_devs.h"

#ifdef DEBUG
#include <errno.h>
#endif

/**
 * @ingroup Drivers
 * @defgroup DLinuxjoy Linux joystick driver
 * @brief GNU/Linux joystick driver
 *
 * Joystick driver for GNU/Linux systems using the v1.0+ joystick API
 * supported by kernels 2.4+. This driver talks directly to the
 * joystick /dev/input/jsX character device(s) and supports up to 32
 * joysticks (X=0..31).
 */

// Bootstrap global
oi_bootstrap linuxjoy_bootstrap = {
  "linuxjoy",
  "GNU/Linux joystick driver",
  OI_PRO_JOYSTICK,
  linuxjoy_avail,
  linuxjoy_device
};

// Joystick to be initialized next
static uchar init_next = 0;

/* ******************************************************************** */
 
/**
 * @ingroup DLinuxjoy
 * @brief Check if POSIX signals exists
 *
 * @param flags library initialization flags, see @ref PFlags
 * @returns errorcode, see @ref PErrors
 *
 * This is a bootstrap function.
 *
 * The easiest test is to check whether the character devices
 * exist. If a single device is found, we assume it's available.
 */
sint linuxjoy_avail(uint flags) {
  sint i;
  sint fd;

  debug("linuxjoy_avail");

  // Simply try to open a joystick
  fd = -1;
  for(i=init_next; i<DLJS_MAX_DEVS; i++) {
    fd = linuxjoy_getfd(i);

    debug("linuxjoy_avail: testing /dev/input/js%u, fd:%i", i, fd);

    // Ok, got on, close it and bail
    if(fd != -1) {
      close(fd);
      break;
    }
  }

  // If filedescriptor isn't -1, a joystick exists
  return (fd != -1);
}

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @brief Create joystick device driver interface
 *
 * @returns pointer to device interface, see @ref IDevstructs
 *
 * This is a bootstrap function.
 *
 * Create the device interface. Even though this driver supports
 * multiple devices, the create-function is just a factory.
 * Device parameters are overridden in the device->init() function.
 */
oi_device *linuxjoy_device() {
  oi_device *dev;
  linuxjoy_private *priv;

  debug("linuxjoy_device");

  // Alloc device data
  dev = (oi_device*)malloc(sizeof(oi_device));
  priv = (linuxjoy_private*)malloc(sizeof(linuxjoy_private));
  if((dev == NULL) || (priv == NULL)) {
    debug("linuxjoy_device: device creation failed");
    if(dev) {
      free(dev);
    }
    if(priv) {
      free(priv);
    }
    return NULL;
  }

  // Clear structures
  memset(dev, 0, sizeof(oi_device));
  memset(priv, 0, sizeof(linuxjoy_private));

  // Set members
  dev->private = priv;
  dev->init = linuxjoy_init;
  dev->destroy = linuxjoy_destroy;
  dev->process = linuxjoy_process;  
  dev->grab = NULL;
  dev->hide = NULL;
  dev->warp = NULL;
  dev->winsize = NULL;
  dev->reset = linuxjoy_reset;
  
  // Done
  return dev;
}

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @brief Initialize the joystick driver
 *
 * @param dev pointer to created device interface
 * @param window_id window hook parameters, see @ref PWindow
 * @param flags initialization flags, see @ref PFlags
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Try to open next joystick.
 */
sint linuxjoy_init(oi_device *dev, char *window_id, uint flags) {
  sint i;
  sint fd;
  char *desc;
  linuxjoy_private *priv;

  debug("linuxjoy_init");

  // We can handle more than one device!
  device_moreavail(TRUE);

  // Find next
  fd = -1;
  for(i=init_next; i<DLJS_MAX_DEVS; i++) {
    fd = linuxjoy_getfd(i);
    if(fd != -1) {
      break;
    }
  }

  // No matter what, don't try this device again
  init_next++;

  // Bail now if no fd was found
  if(fd == -1) {
    return OI_ERR_NO_DEVICE;
  }
  debug("linuxjoy_init: file opened");

  // Ok, time to fetch the greasy device details
  priv = (linuxjoy_private*)dev->private;
  priv->fd = fd;
  priv->id = i;
  
  // Get description using IOCTL (see linux/Documentation/input/joystick-api.h)
  desc = (char*)malloc(DLJS_DESC_SIZE);
  memset(desc, 0, DLJS_DESC_SIZE);
  if(ioctl(fd, JSIOCGNAME(DLJS_DESC_SIZE), desc) < 0) {
    sprintf(desc, "Unknown joystick #%u", i);
  }
  debug("linuxjoy_init: kernel description '%s'", desc);

  // Get number of axes using IOCTL
  if(ioctl(fd, JSIOCGAXES, &priv->num_axes) < 0) {
    priv->num_axes = 2;
  }
  debug("linuxjoy_init: axes '%u'", priv->num_axes);
  
  // Get number of buttons using IOCTL
  if(ioctl(fd, JSIOCGBUTTONS, &priv->num_buttons) < 0) {
    priv->num_buttons = 2;
  }
  debug("linuxjoy_init: buttons '%u'", priv->num_buttons);

  /* See if joystick is run by the  "generic analog" kernel driver.
   * If so, we can get the number of axes/buttons/hats by decoding
   * the name. Kernel name is "Analog X-axis Y-button Z-hat".
   */
  if((strstr(desc, "Analog")==desc) && strstr(desc, "-hat")) {
    int nax;
    int nha;
    // Try to decode number of axes and hats
    if(sscanf(desc, "Analog %d-axis %d-button %*d-hat", &nax, &nha) == 2) {
      priv->num_axes = nax;
      priv->num_hats = nha;
    }
  }
  
  /* Next step is to check if the joystick is a special case we know
   * about
   */
  for(i=0; i<TABLESIZE(linuxjoy_specials); i++) {
    if(strcmp(desc, linuxjoy_specials[i].name) == 0) {
      // Ok, it matches - transfer the settings
      priv->num_axes = linuxjoy_specials[i].num_axes;
      priv->num_hats = linuxjoy_specials[i].num_hats;
      priv->num_balls = linuxjoy_specials[i].num_balls;
    }
  }

  /* //TODO //FIXME
   * SDL supports user-configuration of joysticks using
   * environment variables - there must be a nicer way!?!
   * Also, should we prepare more stuff? Hats? Balls? Buttons?
   * Throttles? Sticks? Axes mapping tables?
   */

  // Store device name and description
  priv->name = (char*)malloc(DLJS_NAME_SIZE);
  sprintf(priv->name, "linuxjoy%u%u",  priv->id/10, priv->id%10);
  priv->desc = desc;
  debug("linuxjoy_init: custom name '%s'", priv->desc);

  // Make the device-structure use the custom name/description
  dev->name = priv->name;
  dev->desc = priv->desc;

  // Done at last!
  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @brief Destroy the joystick device driver
 *
 * @param dev pointer to device interface
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Free device structure and reset signal handlers to
 * the system default.
 */
sint linuxjoy_destroy(oi_device *dev) {
  linuxjoy_private *priv;

  debug("linuxjoy_destroy");
  
  // Free device
  if(dev) {

    // Firstly, free the private data
    priv = (linuxjoy_private*)dev->private;
    if(priv) {
      // Close file
      if(priv->fd != -1) {
	close(priv->fd);
      }

      // Free custom name and description
      if(priv->name) {
	free(priv->name);
      }
      if(priv->desc) {
	free(priv->desc);
      }
    }

    free(dev);
    dev = NULL;
  }

  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @brief Process events
 *
 * @param dev pointer to device interface
 *
 * This is a device interface function.
 *
 * Pump an OI_QUIT event into the queue if a
 * signal is pending.
 */
void linuxjoy_process(oi_device *dev) {
  static struct js_event jse;
  int i;
  linuxjoy_private *priv;

  if(!oi_runstate()) {
    debug("linuxjoy_process: oi_running false");
    return;
  }
  
  // Prepare
  priv = (linuxjoy_private*)dev->private;

  // We're in non-blocking mode, so empty the event queue
  while((i = read(priv->fd, &jse, sizeof(struct js_event))) > 0) {
    // Button
    if(jse.type & JS_EVENT_BUTTON) {
      //FIXME inject event into joystick state manager
    }
    
    // Axis
    else if(jse.type & JS_EVENT_AXIS) {
      //FIXME inject event into joystick state manager
    }
  }

  // Do some debugging
  debug("joystick_process: errorcode '%i'", errno);
}

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @brief Get file descriptor for numbered joystick
 *
 * @param num joystick index (0-31)
 * @returns file descriptor or -1 if joystick not found
 *
 * Open /dev/input/jsX with X=num and return the file descriptor.
 */
int linuxjoy_getfd(uchar num) {
  char path[128];

  // Dummy
  if(num >= DLJS_MAX_DEVS) {
    return -1;
  }

  // Make path and open it
  sprintf(path, "/dev/input/js%u", num);
  return open(path, O_RDONLY | O_NONBLOCK, 0);
}

/* ******************************************************************** */

/**
 * @ingroup DLinuxjoy
 * @brief Reset internal state
 *
 * @param dev pointer to device interface
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Resync the driver-device states.
 */
sint linuxjoy_reset(oi_device *dev) {
  linuxjoy_private *priv;
  priv = (linuxjoy_private*)dev->private;
  debug("linuxjoy_reset");

  // Fixme

  return OI_ERR_OK;
}

/* ******************************************************************** */
