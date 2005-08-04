/*
 * foo.c : Foo (test) utility functions (bootstrapping, etc.)
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
#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "bootstrap.h"
#include "foo.h"

// Bootstrap global
oi_bootstrap foo_bootstrap = {
  "foo",
  "Foo test system",
  OI_PRO_UNKNOWN,
  foo_avail,
  foo_device
};

// Private data
typedef struct foo_private {
  sint grabstatus;
  sint cursorstatus;
  sint x;
  sint y;
} foo_private;

/* ******************************************************************** */
 
// Check availablity of foo
sint foo_avail(uint flags) {
  debug("foo_avail");

  return TRUE;
}

/* ******************************************************************** */

// Install foo 'device' block
oi_device *foo_device() {
  oi_device *dev;
  foo_private *priv;

  debug("foo_device");

  // Alloc device and private data
  dev = (oi_device*)malloc(sizeof(oi_device));
  priv = (foo_private*)malloc(sizeof(foo_private));
  if((dev == NULL) || (priv == NULL)) {
    debug("foo_device: device creation failed");
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
  memset(priv, 0, sizeof(foo_private));

  // Set members
  dev->init = foo_init;
  dev->destroy = foo_destroy;
  dev->process = foo_process;  
  dev->grab = foo_grab;
  dev->hide = foo_hidecursor;
  dev->warp = foo_warp;
  dev->winsize = foo_winsize;
  dev->private = priv;
  
  // Done
  return dev;
}

/* ******************************************************************** */

// Initialize foo
sint foo_init(oi_device *dev, char *window_id, uint flags) {
  uint val;
  foo_private *priv;

  debug("foo_init: window '%s', flags %i", window_id, flags);
  
  // Sniff the handles
  val = device_windowid(window_id, OI_I_CONN);
  debug("foo_init: conn (c) parameter %i", val);

  val = device_windowid(window_id, OI_I_SCRN);
  debug("foo_init: scrn (s) parameter %i", val);

  val = device_windowid(window_id, OI_I_WINID);
  debug("foo_init: winid (w) parameter %i", val);

  // Set some stupid private values
  priv = (foo_private*)dev->private;
  priv->grabstatus = FALSE;
  priv->cursorstatus = FALSE;
  priv->x = 0;
  priv->y = 0;

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Free foo device structure
sint foo_destroy(oi_device *dev) {
  debug("foo_destroy");
  
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

// Pump events into event queue
void foo_process(oi_device *dev) {
  static oi_event ev;

  debug("foo_process");

  if(!oi_runstate()) {
    debug("foo_process: oi_running false");
    return;
  }

  // Since this is a test device, generate an event
  ev.type = OI_KEYDOWN;
  ev.key.device = dev->index;
  ev.key.state = 1;
  ev.key.keysym.scancode = 65;
  ev.key.keysym.sym = SK_A;
  ev.key.keysym.mod = SM_NONE;
  
  // Post event
  queue_add(&ev);
}

/* ******************************************************************** */

// Pump events into event queue
sint foo_grab(oi_device *dev, sint on) {
  foo_private *priv;
  priv = (foo_private*)dev->private;
  debug("foo_grab: current:%i new:%i", priv->grabstatus, on);
  priv->grabstatus = on;

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Show/hide pointer cursor
sint foo_hidecursor(oi_device *dev, sint on) {
  foo_private *priv;
  priv = (foo_private*)dev->private;
  debug("foo_hidecursor: current:%i new:%i", priv->cursorstatus, on);
  priv->cursorstatus = on;
  
  return OI_ERR_OK;
}

/* ******************************************************************** */

// Warp pointer
sint foo_warp(oi_device *dev, sint x, sint y) {
  foo_private *priv;
  priv = (foo_private*)dev->private;
  debug("foo_warp: warp pointer to %i, %i", x, y);
  priv->x = x;
  priv->y = y;

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Window size notifier
sint foo_winsize(oi_device *dev, sint *w, sint *h) {
  foo_private *priv;
  priv = (foo_private*)dev->private;
  debug("foo_winsize");

  // Just for fun, return the mouse position
  *w = priv->x;
  *h = priv->y;

  return OI_ERR_OK;
}

/* ******************************************************************** */
