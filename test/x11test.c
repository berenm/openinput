/*
 * x11test.c : X11 test driver
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
#include "sinp.h"
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
  printf("q => quit\n");
  printf("\n");
}

/* ******************************************************************** */

// The libsinp test
void test(Display *d, Window w, uint scrn) {
  int e;
  sinp_event ev;
  char csw[100];
  int sgrab;
  int scursor;

  // Init sinp
  sprintf(csw, "c:%u s:%u w:%u", (uint)d, (uint)scrn, (uint)w);
  printf("--- init parameter '%s'\n", csw);

  e = sinp_init(csw, 0);
  printf("--- sinp_init, code %i\n\n", e);

  // States
  sgrab = 1;
  scursor = 1;

  // Print something
  help();

  // Try to fetch an event
  e = 1;
  while(e) {
    sinp_events_wait(&ev);

    // Quit
    if(ev.type == SINP_QUIT) {
      e = 0;
    }

    // Mouse button up
    else if((ev.type == SINP_MOUSEBUTTONUP) ||
	    (ev.type == SINP_MOUSEBUTTONDOWN)) {
      printf("--- mouse button state %i at position %i,%i\n",
	     ev.button.state, ev.button.x, ev.button.y);
    }
    
    // Mouse move
    else if(ev.type == SINP_MOUSEMOVE) {
      printf("--- mouse move  abs:%i,%i \t rel:%i,%i\n",
	     ev.move.x, ev.move.y,
	     ev.move.relx, ev.move.rely);
    }

    // Key down
    else if(ev.type == SINP_KEYDOWN) {
      printf("--- key pressed -> %i:'%s'\n", ev.key.keysym.sym,
	     sinp_key_getname(ev.key.keysym.sym));
    }
    
    // Key up
    else if(ev.type == SINP_KEYUP) {
      printf("--- key release -> %i:'%s'\n", ev.key.keysym.sym,
	     sinp_key_getname(ev.key.keysym.sym));

      switch(ev.key.keysym.sym) {
      case SK_H:
	// Help
	help();
	break;

      case SK_G:
	// Grab
	toggle(&sgrab);
	sinp_app_grab(sgrab);
	printf("*** grab state %i\n", sgrab);
	break;

      case SK_C:
	// Cursor
	toggle(&scursor);
	if(scursor) {
	  sinp_app_cursor(SINP_ENABLE);
	} else {
	  sinp_app_cursor(SINP_DISABLE);
	}
	printf("*** cursor state %i\n", scursor);
	break;
	
      case SK_W:
	// Warp
	sinp_mouse_warp(10, 10);
     	printf("*** warped\n");
	break;

      case SK_Q:
	// Quit
	e = 0;
	break;

      default:
	// Other
	break;
      }
    }

    // Other event
    else {
      switch(ev.type) {
      case SINP_ACTIVE:
	printf("--- active event\n");
	break;

      case SINP_RESIZE:
	printf("--- resize event\n");
	break;

      case SINP_EXPOSE:
	printf("--- expose event\n");
	break;

      case SINP_DISCOVERY:
	printf("--- discovery event\n");
	break;

      default:
	printf("--- unhandled event type %i\n", ev.type);
	break;
      }
    }
  }

  // Close sinp
  e = sinp_close();
  printf("--- sinp_close, code %i\n\n", e);
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
