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

// The libsinp test
void test(Display *d, Window w, uint scrn) {
  int e;
  sinp_event ev;
  char csw[100];

  // Init sinp
  sprintf(csw, "c:%u s:%u w:%u", (uint)d, (uint)scrn, (uint)w);
  printf("--- init parameter '%s'\n", csw);

  e = sinp_init(csw, 0);
  printf("--- sinp_init, code %i\n\n", e);

  // Try to fetch an event
  while(1) {
    sinp_events_wait(&ev);
    printf("--- event type %i\n\n", ev.type);

    // Quit
    if(ev.type == SINP_QUIT) {
      break;
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
