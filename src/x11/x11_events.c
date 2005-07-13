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

  // Bruteforce-attack the X server to make it talk (thanks SDL!)
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
inline void x11_dispatch(sinp_device *dev, Display *d) {
  XEvent xev;
  
  // Fetch the event
  XNextEvent(d, &xev);
  
  // Handle
  switch(xev.type) {
  
    
    // Mouse enters/leaves window
  case EnterNotify:
  case LeaveNotify:
    debug("x11_dispatch: enter/leave_notify");
    
    // We're not interested in grab mode events
    if((xev.xcrossing.mode != NotifyGrab) &&
       (xev.xcrossing.mode != NotifyUngrab)) {
      
      // Move if grabbed, otherwise change focus
      if(sinp_app_grab(SINP_QUERY) == SINP_ENABLE) {
	mouse_move(xev.xcrossing.x, xev.xcrossing.y, FALSE, dev->index);
      }
      else {
	appstate_focus(SINP_FOCUS_MOUSE,
		       xev.type == EnterNotify,
		       dev->index);
      }
    }
    break;
    

    // Input focus gained/lost
  case FocusIn:
  case FocusOut:
    debug("x11_dispatch: focus_in/out");
    appstate_focus(SINP_FOCUS_INPUT,
		   xev.type == FocusIn,
		   dev->index);
    break;

    
    // Generated on EnterWindow and FocusIn
  case KeymapNotify:
    debug("x11_dispatch: keymap_notify");
    //FIXME handle full reset of keystate/modstate
    break;


    // Mouse motion
  case MotionNotify:
    debug("x11_dispatch: motion_notify");
    //FIXME handle relative mouse movement
    mouse_move(xev.xmotion.x, xev.xmotion.y, FALSE, dev->index);
    break;


    // Mouse button pressed
  case ButtonPress:
  case ButtonRelease:
    debug("x11_dispatch: button_press/release");
    mouse_button(xev.xbutton.button,
		 xev.type == ButtonPress,
		 dev->index);
    break;


    // Keyboard pressed
  case KeyPress:
  case KeyRelease:
    debug("x11_dispatch: key_press/release");
    break;


    // Window gets iconified
  case UnmapNotify:
    debug("x11_dispatch: unmap_notify");
    appstate_focus(FALSE, SINP_FOCUS_INPUT | SINP_FOCUS_VISIBLE, dev->index);
    break;

    // Window gets restored (uniconified)
  case MapNotify:
    appstate_focus(TRUE, SINP_FOCUS_VISIBLE, dev->index);
    break;


    // Window was resized or moved
  case ConfigureNotify:
    debug("x11_dispatch: configure_notify");
    appstate_resize(xev.xconfigure.width, xev.xconfigure.height, dev->index);		    
    break;
    

    // We should quit -- or other messages
  case ClientMessage:
    debug("x11_dispatch: client_message");
    // Window manager close window
    if((xev.xclient.format == 32) &&
       (xev.xclient.data.l[0] == ((x11_private*)dev->private)->wm_delete_window)) {
      sinp_event ev;
      ev.type = SINP_QUIT;
      queue_add(&ev);
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
