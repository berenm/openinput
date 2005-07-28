/*
 * unixsignal.c : UNIX signal handler (intr, sigsev, etc.)
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
#include <signal.h>
#include "internal.h"
#include "bootstrap.h"
#include "unixsignal.h"

// Bootstrap global
sinp_bootstrap unixsignal_bootstrap = {
  "unixsignal",
  "UNIX signal handler",
  SINP_PRO_UNKNOWN,
  unixsignal_avail,
  unixsignal_device
};

// Private data is global here (it makes no sense with a private struct)
static int pendingsignal;

/* ******************************************************************** */
 
// Check availablity of UNIX ;)
sint unixsignal_avail(uint flags) {
  debug("unixsignal_avail");

  return TRUE;
}

/* ******************************************************************** */

// Install device block
sinp_device *unixsignal_device() {
  sinp_device *dev;

  debug("unixsignal_device");

  // Alloc device data
  dev = (sinp_device*)malloc(sizeof(sinp_device));
  if(dev == NULL) {
    debug("unixsignal_device: device creation failed");
    return NULL;
  }

  // Clear structures
  memset(dev, 0, sizeof(sinp_device));

  // Set members
  dev->init = unixsignal_init;
  dev->destroy = unixsignal_destroy;
  dev->process = unixsignal_process;  
  dev->grab = NULL;
  dev->hide = NULL;
  dev->warp = NULL;
  dev->winsize = NULL;
  dev->private = NULL;
  
  // Done
  return dev;
}

/* ******************************************************************** */

// Initialize device
sint unixsignal_init(sinp_device *dev, char *window_id, uint flags) {
  debug("unixsignal_init");

  // Just to be sure, no signal is pending
  pendingsignal = FALSE;
 
  // Install handler for various shutdown-signals
  signal(SIGINT, unixsignal_handler);  // Interrupt (ctrl+c)
  signal(SIGTERM, unixsignal_handler); // Terminate (kill)
  signal(SIGSEGV, unixsignal_handler); // Segfault

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Free device structure
sint unixsignal_destroy(sinp_device *dev) {
  debug("unixsignal_destroy");
  
  // Free device
  if(dev) {
    free(dev);
    dev = NULL;
  }

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Pump events into event queue
void unixsignal_process(sinp_device *dev) {
  static sinp_event ev;

  if(!sinp_runstate()) {
    debug("unixsignal_process: sinp_running false");
    return;
  }

  // Bail out if no signal
  if(!pendingsignal) {
    return;
  }

  // Don't forget to clear the flag
  pendingsignal = FALSE;

  // A signal, send quit event
  ev.type = SINP_QUIT;
  queue_add(&ev);
}

/* ******************************************************************** */

// The POSIX signal handler
void unixsignal_handler(int signum) {

  // Ok, we've fetched a signal
  pendingsignal = TRUE;
  debug("unixsignal_handler: signal %d received", signum);  
}

/* ******************************************************************** */
