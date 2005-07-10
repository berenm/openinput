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
#include "config.h"
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

// Private data
typedef struct foo_private {
  uint grabmask;
} foo_private;

/* ******************************************************************** */
 
// Check availablity of foo
sint foo_avail() {
  debug("foo_avail");

  return TRUE;
}

/* ******************************************************************** */

// Install foo 'device' block
sinp_device *foo_device() {
  sinp_device *dev;
  foo_private *priv;

  debug("foo_device");

  // Alloc device and private data
  dev = (sinp_device*)malloc(sizeof(sinp_device));
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

  // Set members
  memset(dev, 0, sizeof(sinp_device));
  dev->init = foo_init;
  dev->enable = foo_enable;
  dev->destroy = foo_destroy;
  dev->process = foo_process;  
  dev->grab = foo_grab;
  dev->private = priv;
  
  // Done
  return dev;
}

/* ******************************************************************** */

// Initialize foo
sint foo_init(sinp_device *dev, char *window_id, uint flags) {
  uint val;

  debug("foo_init: window '%s', flags %i", window_id, flags);
  
  // Sniff the handles
  val = device_windowid(window_id, SINP_I_CONN);
  debug("foo_init: conn (c) parameter %i", val);

  val = device_windowid(window_id, SINP_I_SCRN);
  debug("foo_init: scrn (s) parameter %i", val);

  val = device_windowid(window_id, SINP_I_WINID);
  debug("foo_init: winid (w) parameter %i", val);

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Enable foo driver
sint foo_enable(sinp_device *dev, sint on) {
  debug("foo_enable to %i", on);
  
  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Free foo device structure
sint foo_destroy(sinp_device *dev) {
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

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Pump events into event queue
void foo_process(sinp_device *dev) {
  static sinp_event ev;

  debug("foo_process");

  // Since this is a test device, generate an event
  ev.type = SINP_KEYDOWN;
  ev.key.device = dev->index;
  ev.key.state = 1;
  ev.key.keysym.scancode = 65;
  ev.key.keysym.sym = SK_A;
  ev.key.keysym.mod = SM_NONE;
  ev.key.keysym.unicode = 0;
  
  // Post event
  queue_add(&ev);
}

/* ******************************************************************** */

// Pump events into event queue
sint foo_grab(sinp_device *dev, uint mask) {
  foo_private *priv;
  sint ok;

  priv = (foo_private*)dev->private;
  ok = FALSE;
  debug("foo_grab");

  // Toggle
  if(mask & SINP_PRO_UNKNOWN) {
    priv->grabmask |= mask;
    ok = TRUE;
  }
  if(!(mask & SINP_PRO_UNKNOWN)) {
    priv->grabmask = priv->grabmask - (priv->grabmask & mask);
    ok = TRUE;
  }

  // Be nice to the user - report errors
  if(ok) {
    return SINP_ERR_OK;
  }
  else {
    return SINP_ERR_PARAM;
  }
}

/* ******************************************************************** */
