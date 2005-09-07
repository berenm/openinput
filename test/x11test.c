/*
 * x11test.c : X11 test driver
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
#include "openinput.h"
#include <X11/Xlib.h>
#include <stdio.h>

/* ******************************************************************** */

// Bit toggler
void toggle(int *i) {
  if(*i) {
    *i = 0;
  }
  else {
    *i = 1;    
  }
}

/* ******************************************************************** */

// Show some help
void help() {
  printf("--- x11test help\n");
  printf("h => show this test\n");
  printf("g => toggle grab\n");
  printf("c => toggle cursor\n");
  printf("w => warp to 10,10\n");
  printf("m => print mouse state\n");
  printf("e => toggle device 2 on/off\n");
  printf("q => quit\n");
  printf("\n");
}

/* ******************************************************************** */

// Some device debugging for discovery events
void debug_discovery(uint index) {
  char *name;
  char *desc;
  uint pro;

  // Fetch info
  oi_device_info(index, &name, &desc, &pro);
  printf("*** device info for device index %u\n", index);
  printf("    name:     '%s'\n", name);
  printf("    desc:     '%s'\n", desc);
  printf("    provides: '0x%X'\n", pro);
}

/* ******************************************************************** */

// The openinput test
void test(Display *d, Window w, uint scrn) {
  int e;
  oi_event ev;
  char csw[100];
  int sgrab;
  int scursor;
  int senable;

  // Init OI
  sprintf(csw, "c:%u s:%u w:%u", (uint)d, (uint)scrn, (uint)w);
  printf("--- init parameter '%s'\n", csw);

  e = oi_init(csw, 0);
  printf("--- oi_init, code %i\n\n", e);

  // States
  sgrab = 1;
  scursor = 1;
  senable = 1;

  // Print something
  help();

  // Try to fetch an event
  e = 1;
  while(e) {
    oi_events_wait(&ev);

    printf("--- got event\n");

    // Quit
    if(ev.type == OI_QUIT) {
      e = 0;
    }

    // Mouse button up
    else if((ev.type == OI_MOUSEBUTTONUP) ||
	    (ev.type == OI_MOUSEBUTTONDOWN)) {
      printf("--- mouse button state %i at position %i,%i\n",
	     ev.button.state, ev.button.x, ev.button.y);
    }
    
    // Mouse move
    else if(ev.type == OI_MOUSEMOVE) {
      printf("--- mouse move  abs:%i,%i \t rel:%i,%i\n",
	     ev.move.x, ev.move.y,
	     ev.move.relx, ev.move.rely);
    }

    // Key down
    else if(ev.type == OI_KEYDOWN) {
      printf("--- key pressed -> %i:'%s'\n", ev.key.keysym.sym,
	     oi_key_getname(ev.key.keysym.sym));
    }
    
    // Key up
    else if(ev.type == OI_KEYUP) {
      printf("--- key release -> %i:'%s'\n", ev.key.keysym.sym,
	     oi_key_getname(ev.key.keysym.sym));

      switch(ev.key.keysym.sym) {
      case OIK_H:
	// Help
	help();
	break;

      case OIK_G:
	// Grab
	toggle(&sgrab);
	oi_app_grab(sgrab);
	printf("*** grab state %i\n", sgrab);
	break;

      case OIK_C:
	// Cursor
	toggle(&scursor);
	if(scursor) {
	  oi_app_cursor(OI_ENABLE);
	} else {
	  oi_app_cursor(OI_DISABLE);
	}
	printf("*** cursor state %i\n", scursor);
	break;
	
      case OIK_M:
	// Print mouse state
	{
	  int x,y;
	  oi_mouse_absolute(0, &x, &y);
	  printf("*** mouse absolute: %i,%i\n", x, y);
	  oi_mouse_relative(0, &x, &y);
	  printf("*** mouse relative: %i,%i\n", x, y);
	}
	break;

      case OIK_W:
	// Warp
	oi_mouse_warp(0, 10, 10);
     	printf("*** warped\n");
	break;

      case OIK_Q:
	// Quit
	e = 0;
     	printf("*** quit!\n");
	break;

      case OIK_E:
	// Enable/disable device index 2
	toggle(&senable);
	oi_device_enable(2, senable);
     	printf("*** device 2 state %i\n", senable);
	break;

      default:
	// Other
	break;
      }
    }

    // Other event
    else {
      switch(ev.type) {
      case OI_ACTIVE:
	printf("--- active event\n");
	break;

      case OI_RESIZE:
	printf("--- resize event\n");
	break;

      case OI_EXPOSE:
	printf("--- expose event\n");
	break;

      case OI_DISCOVERY:
	printf("--- discovery event\n");
	debug_discovery(ev.discover.device);
	break;

      default:
	printf("--- unhandled event type %i\n", ev.type);
	break;
      }
    }
  }

  // Close OI
  e = oi_close();
  printf("--- oi_close, code %i\n\n", e);
}

/* ******************************************************************** */

// Main function
int main(int argc, char* argv[]) {
  Display *disp;
  Window win;
  XEvent evt;
  uint scrn;
  long event_mask;

  // Startup
  printf("*** x11test start ***\n\n");

  // Open connection
  disp = XOpenDisplay( NULL );
  if(!disp) {
    printf("--- could not open display\n");
    return 1;
  }

  // Open and map window
  scrn = DefaultScreen(disp);
  win = XCreateSimpleWindow(disp, DefaultRootWindow(disp),
				   50, 50,
				   200, 200,
				   0, BlackPixel(disp, scrn),
				   WhitePixel(disp, scrn));
  XMapWindow(disp, win);

  // Wait for map event
  event_mask = StructureNotifyMask;
  XSelectInput(disp, win, event_mask);
  do {
    XNextEvent(disp, &evt);
  } while(evt.type != MapNotify);

  // Perform the test
  test(disp, win, scrn);
  
  // Close window and connection
  XDestroyWindow(disp, win);
  XCloseDisplay(disp);

  printf("*** x11test stop ***\n\n");

  return 0;
}
