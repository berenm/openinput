/*
 * x11.c : X11 utility functions (bootstrapping, etc.)
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
#include <unistd.h>
#include <X11/Xlib.h>
#include "internal.h"
#include "bootstrap.h"
#include "x11.h"

// Bootstrap global
oi_bootstrap x11_bootstrap = {
  "x11",
  "X11 Window system",
  OI_PRO_KEYBOARD | OI_PRO_MOUSE | OI_PRO_WINDOW,
  x11_avail,
  x11_device,
};

/* ******************************************************************** */
 
// Bootstrap: Check availablity of X11
sint x11_avail(uint flags) {
  Display *disp;

  debug("x11_avail");

  // Check flags
  if(flags & OI_FLAG_NOWINDOW) {
    return FALSE;
  }

  // Check for X11 existence
  disp = XOpenDisplay(NULL);
  if(disp != NULL) {
    // Got it, close display again
    XCloseDisplay(disp);
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/* ******************************************************************** */

// Bootstrap: Install X11 'device' block
oi_device *x11_device() {
  oi_device *dev;
  x11_private *priv;

  debug("x11_device");

  // Alloc
  dev = (oi_device*)malloc(sizeof(oi_device));
  priv = (x11_private*)malloc(sizeof(x11_private));
  if((dev == NULL) || (priv == NULL)) {
    debug("x11_device: device creation failed");
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
  memset(priv, 0, sizeof(x11_private));

  // Set members
  dev->private = priv;
  dev->init = x11_init;
  dev->destroy = x11_destroy;
  dev->process = x11_process;  
  dev->grab = x11_grab;
  dev->hide = x11_hidecursor;
  dev->warp = x11_warp;
  dev->winsize = x11_winsize;
  
  // Done
  return dev;
}

/* ******************************************************************** */

// Initialize X11
sint x11_init(oi_device *dev, char *window_id, uint flags) {
  x11_private *priv;

  priv = (x11_private*)dev->private;
  debug("x11_init");

  // Parse the window_id flags
  priv->disp = (Display*)device_windowid(window_id, OI_I_CONN);
  priv->screen = (Screen*)device_windowid(window_id, OI_I_SCRN);
  priv->win = (Window)device_windowid(window_id, OI_I_WINID);

  // We require conn and winid parameters
  if(!(priv->disp) || !(priv->win)) {
    debug("x11_init: conn (c) and winid (w) parameters required\n");
    return OI_ERR_NO_DEVICE;
  }

  // Install error handlers
  XSetErrorHandler(x11_error);
  XSetIOErrorHandler(x11_fatal);

  // Initialize blank-cursor, keymapper table, modifier mask and key state
  priv->cursor = x11_mkcursor(priv->disp, priv->win);
  priv->relative = 0;
  x11_initkeymap();
  x11_modmasks(priv->disp, dev);
  x11_keystate(dev, priv->disp, NULL);
  
  // Start receiving events
  XSelectInput(priv->disp, priv->win, FocusChangeMask | KeyPressMask |
	       KeyReleaseMask | PropertyChangeMask | StructureNotifyMask |
	       KeymapStateMask | ButtonPressMask | ButtonReleaseMask |
	       PointerMotionMask | EnterWindowMask | LeaveWindowMask );

  // Get "close window" window manager protocol atom
  priv->wm_delete_window = XInternAtom(priv->disp, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(priv->disp, priv->win, &(priv->wm_delete_window), 1);  

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Clear X11 'device' block
sint x11_destroy(oi_device *dev) {
  x11_private *priv;

  debug("x11_destroy");

  if(dev) {
    priv = (x11_private*)dev->private;

    // Private members
    if(priv) {
      if(priv->cursor) {
	XFreeCursor(priv->disp, priv->cursor);
      }
      free(priv);
    }

    // Device struct
    free(dev);
    dev = NULL;
  }
  
  return OI_ERR_OK;
}

/* ******************************************************************** */

// Pump event into the queue
void x11_process(oi_device *dev) {
  x11_private *priv;

  priv = (x11_private*)dev->private;

  // Process all pending events
  while(oi_runstate() && x11_pending(priv->disp)) {
    x11_dispatch(dev, priv->disp);
  }
}

/* ******************************************************************** */

// Grab (lock) mouse/keyboard in window
sint x11_grab(oi_device *dev, sint on) {
  x11_private *priv;
  int i;

  priv = (x11_private*)dev->private;
  debug("x11_grab: state:%i", on);

  if(on) {
    // Raise window and focus it
    XRaiseWindow(priv->disp, priv->win);
    XSetInputFocus(priv->disp, priv->win, RevertToParent, CurrentTime);

    // Grab input
    XGrabKeyboard(priv->disp, priv->win, TRUE,
		  GrabModeAsync, GrabModeAsync, CurrentTime);
    
    // Wait for succesfull grabbing of mouse 
    while(1) {
      i = XGrabPointer(priv->disp, priv->win, TRUE,
		       ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
		       GrabModeAsync, GrabModeAsync,
		       priv->win, None, CurrentTime);
      if(i == GrabSuccess) {
	break;
      }
      usleep(OI_SLEEP);
    }

    // Set flag for possible relative mouse
    priv->relative |= SX11_GRAB;
  }
  else {
    // Simply ungrab both
    XUngrabKeyboard(priv->disp, CurrentTime);
    XUngrabPointer(priv->disp, CurrentTime);

    // Fix relative mouse motion
    priv->relative &= ~SX11_GRAB;
  }

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Show/hide mouse cursor
sint x11_hidecursor(oi_device *dev, sint on) {
  x11_private *priv;

  debug("x11_hidecursor: state:%i", on);
  priv = (x11_private*)dev->private;

  // Hide - set blank cursor
  if(on) {
    XDefineCursor(priv->disp, priv->win, priv->cursor);
    priv->relative |= SX11_HIDE;
  }

  // Show - set default cursor
  else {
    XDefineCursor(priv->disp, priv->win, None);
    priv->relative &= ~SX11_HIDE;
  }

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Warp mouse
sint x11_warp(oi_device *dev, sint x, sint y) {
  x11_private *priv;

  priv = (x11_private*)dev->private;

  // Simply go to warp speed
  XWarpPointer(priv->disp, None, priv->win, 0, 0, 0, 0, x, y);
  XSync(priv->disp, False);

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Get window sizes
sint x11_winsize(oi_device *dev, sint *w, sint *h) {
  x11_private *priv;
  XWindowAttributes attr;

  priv = (x11_private*)dev->private;
  
  XGetWindowAttributes(priv->disp, priv->win, &attr);

  *w = attr.width;
  *h = attr.height;
  priv->width = *w;
  priv->height = *h;

  debug("x11_winsize: width:%i height:%i", *w, *h);
  
  return OI_ERR_OK;
}

/* ******************************************************************** */

// X11 error
sint x11_error(Display *d, XErrorEvent *e) {
  debug("x11_error: code %u", e->error_code);

  return OI_ERR_OK;
}

/* ******************************************************************** */

// X11 fatal I/O error
sint x11_fatal(Display *d) {
  debug("x11_fatal: fatal I/O error");

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Make 'empty' mouse cursor
Cursor x11_mkcursor(Display *d, Window w) {
  Pixmap pixmap;
  XColor color;
  Colormap colmap;
  Cursor cursor;
  
  // Make 1x1 pixmap
  pixmap = XCreatePixmap(d, DefaultRootWindow(d), 1, 1, 1);
  
  // Get the color black
  colmap = XCreateColormap(d, DefaultScreen(d),
			   DefaultVisual(d, DefaultScreen(d)),
			   AllocNone);
  XParseColor(d, colmap, "black", &color);
  
  // Create the cursor
  cursor = XCreatePixmapCursor(d, pixmap, pixmap,
			 &color, &color, 1, 1);

  // Free and sync
  XFreePixmap(d, pixmap);
  XFreeColormap(d, colmap);
  XSync(d, False);
  
  return cursor;
}

/* ******************************************************************** */
