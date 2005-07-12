/*
 * x11_translate.c : Overall X11 event handler
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
#include <string.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include "internal.h"
#include "x11.h"

/* ******************************************************************** */

// Check for pending events without going into a long-time blocking call
inline sint x11_pending(Display *d) {
  // Flush to pump the X pipeline
  XFlush(d);
  
  // Standard X non-blocking check
  if(XEventsQueued(d, QueuedAlready)) {
    return TRUE;
  }

  // Bruteforce-attack the X server to make it take (thanks SDL!)
  {
    static struct timeval time;
    int fd;
    fd_set set;
    
    fd = ConnectionNumber(d);
    FD_ZERO(&set);
    FD_SET(fd, &set);
    
    // Charge!
    if(select(fd+1, &set, NULL, NULL, &time) == 1) {
      // Ok, perform the blocking X call
      return XPending(d);
    }
  }

  // Nothing is pending
  return FALSE;
}

/* ******************************************************************** */

// Dispatch a pending X11 event into the sinp event queue
inline void x11_dispatch(Display *d) {
  XEvent xev;
  
  // Fetch the event
  XNextEvent(d, &xev);
  
  // Handle
  switch(xev.type) {
    
    // Mouse enters/leaves window
  case EnterNotify:
  case LeaveNotify:
    {
      debug("x11_dispatch: enternotify / leavenotify");

      // We're not interested in grab mode events
      if((xev.xcrossing.mode != NotifyGrab) &&
	 (xev.xcrossing.mode != NotifyUngrab)) {
	
	// If grab is off, mouse either left or entered window
	if(1) {
	  //FIXME here
	}
      }
    }
    break;

    // Input focus gained
  case FocusIn: {
    debug("x11_dispatch: focusin");
  }
    break;

    // Input focus lost
  case FocusOut: {
    debug("x11_dispatch: focusout");
  }
    break;

    // Generated on EnterWindow and FocusIn
  case KeymapNotify: {
    debug("x11_dispatch: keymapnotify");
  }
    break;

    // Mouse motion
  case MotionNotify: {
    debug("x11_dispatch: motionnotify");
  }
    break;

    // Mouse button pressed
  case ButtonPress: {
    debug("x11_dispatch: buttonpress");
  }
    break;

    // Mouse button released
  case ButtonRelease: {
    debug("x11_dispatch: buttonrelease");
  }
    break;

    // Keyboard pressed
  case KeyPress: {
    debug("x11_dispatch: keypress");
  }
    break;

    // Keyboard released
  case KeyRelease: {
    debug("x11_dispatch: keyrelease");
  }
    break;

    // Window gets iconified
  case UnmapNotify: {
    debug("x11_dispatch: unmapnotify");
  }
    break;

    // Window gets restored (uniconified)
  case MapNotify: {
    debug("x11_dispatch: mapnotify");
  }
    break;

    // Window was resized or moved
  case ConfigureNotify: {
    debug("x11_dispatch: configurenotify");
  }
    break;
    
    // We should quit -- or other messages
  case ClientMessage: {
    debug("x11_dispatch: clientmessage");
  }
    break;

    // Redraw required
  case Expose: {
    debug("x11_dispatch: expose");
  }
    break;
  }
}

/* ******************************************************************** */
