/*
 * win32.h : Microsoft Windows input device driver
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
#include <string.h>
#include <stdlib.h>
#include "openinput.h"
#include "internal.h"
#include "bootstrap.h"
#include "win32.h"

/**
 * @ingroup Drivers
 * @defgroup DWin32 Microsoft Windows input driver
 *
 * @todo Document this!
 */

// Bootstrap global
oi_bootstrap win32_bootstrap = {
  "win32",
  "Microsoft Windows input system",
  OI_PRO_MOUSE | OI_PRO_KEYBOARD,
  win32_avail,
  win32_device
};


/* ******************************************************************** */
 
/**
 * @ingroup DWin32
 * @brief Check availablity of win32 device
 *
 * @param flags initialization flags, see @ref PFlags
 * @returns true (1) if device is present, false (0) otherwise
 *
 * This is a bootstrap function.
 *
 * @todo Document this!
 */
sint win32_avail(uint flags) {
  debug("win32_avail");

  return FALSE;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Create win32 device structure
 *
 * @returns pointer to allocated device interface
 *
 * This is a bootstrap function.
 * 
 * @todo Document this!
 */
oi_device *win32_device() {
  oi_device *dev;
  win32_private *priv;

  debug("win32_device");

  // Alloc device and private data
  dev = (oi_device*)malloc(sizeof(oi_device));
  priv = (win32_private*)malloc(sizeof(win32_private));
  if((dev == NULL) || (priv == NULL)) {
    debug("win32_device: device creation failed");
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
  memset(priv, 0, sizeof(win32_private));

  // Set members
  dev->init = win32_init;
  dev->destroy = win32_destroy;
  dev->process = win32_process;  
  dev->grab = win32_grab;
  dev->hide = win32_hidecursor;
  dev->warp = win32_warp;
  dev->winsize = win32_winsize;
  dev->reset = win32_reset;
  dev->private = priv;
  
  // Done
  return dev;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Initialize the win32 test driver
 *
 * @param dev pointer to device interface
 * @param window_id window hook paramaters, see @ref PWindow
 * @param flags initialization flags, see @ref PFlags
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * @todo Document this!
 */
sint win32_init(oi_device *dev, char *window_id, uint flags) {
  uint val;
  win32_private *priv;

  debug("win32_init: window '%s', flags %i", window_id, flags);
  
  // Sniff the handles
  val = device_windowid(window_id, OI_I_CONN);
  debug("win32_init: conn (c) parameter %i", val);

  val = device_windowid(window_id, OI_I_SCRN);
  debug("win32_init: scrn (s) parameter %i", val);

  val = device_windowid(window_id, OI_I_WINID);
  debug("win32_init: winid (w) parameter %i", val);

  // Set some stupid private values
  priv = (win32_private*)dev->private;
  priv->x = 0;

  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Shutdown the win32 device
 *
 * @param dev pointer to device interface
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * @todo Document this!
 */
sint win32_destroy(oi_device *dev) {
  debug("win32_destroy");
  
  // Free device
  if(dev) {
    // Private data
    if(dev->private) {
      free(dev->private);
    }
    free(dev);
    dev = NULL;
  }

  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Process events
 *
 * @param dev pointer to device interface
 *
 * This is a device interface function.
 *
 * @todo Document this!
 */
void win32_process(oi_device *dev) {
  static oi_event ev;

  debug("win32_process");

  if(!oi_runstate()) {
    debug("win32_process: oi_running false");
    return;
  }

  // Since this is a test device, generate an event
  ev.type = OI_KEYDOWN;
  ev.key.device = dev->index;
  ev.key.state = 1;
  ev.key.keysym.scancode = 65;
  ev.key.keysym.sym = OIK_A;
  ev.key.keysym.mod = OIM_NONE;
  
  // Post event
  queue_add(&ev);
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Grab/release pointer
 *
 * @param dev pointer to device interface
 * @param on true (1) turns on grab, false (0) releases grab
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * @todo Document this!
 */
sint win32_grab(oi_device *dev, sint on) {
  win32_private *priv;
  priv = (win32_private*)dev->private;
  debug("win32_grab");

  return OI_ERR_OK;
}

/* ******************************************************************** */


/**
 * @ingroup DWin32
 * @brief Show/hide pointer
 *
 * @param dev pointer to device interface
 * @param on true (1) hides cursor, false (0) shows cursor
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * @todo Document this!
 */
sint win32_hidecursor(oi_device *dev, sint on) {
  win32_private *priv;
  priv = (win32_private*)dev->private;
  debug("win32_hidecursor");
  
  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Warp pointer
 *
 * @param dev pointer to device interface
 * @param x pointer to horizontal position
 * @param y pointer to vertical position
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * @todo Document this!
 */
sint win32_warp(oi_device *dev, sint x, sint y) {
  win32_private *priv;
  priv = (win32_private*)dev->private;
  debug("win32_warp");

  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Get window size
 *
 * @param dev pointer to device interface
 * @param w pointer to horizontal size
 * @param h pointer to vertical size
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * @todo Document this!
 */
sint win32_winsize(oi_device *dev, sint *w, sint *h) {
  win32_private *priv;
  priv = (win32_private*)dev->private;
  debug("win32_winsize");

  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Reset internal state
 *
 * @param dev pointer to device interface
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * @todo Document this!
 */
sint win32_reset(oi_device *dev) {
  win32_private *priv;
  priv = (win32_private*)dev->private;
  debug("win32_reset");

  return OI_ERR_OK;
}

/* ******************************************************************** */
