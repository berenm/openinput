/*
 * x11actiontest.c : Action mapper test using X11 input
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
#include <stdlib.h>

#define NUM_MAPS 6

/* ******************************************************************** */

// The openinput test
void test() {
  int e;
  int i;
  oi_event ev;
  oi_actionmap *map;

  // Build a simple actionmap
  map = (oi_actionmap*)malloc(sizeof(oi_actionmap) * NUM_MAPS);

  map[0].actionid = 1;
  map[0].device = 0;
  map[0].name = "key_space";

  // Note that we add two key_a's with different actionids
  map[1].actionid = 2;
  map[1].device = 0;
  map[1].name = "key_a";

  map[2].actionid = 6;
  map[2].device = 0;
  map[2].name = "key_a";

  map[3].actionid = 3;
  map[3].device = 0;
  map[3].name = "mouse_motion";

  map[4].actionid = 4;
  map[4].device = 0;
  map[4].name = "mouse_button_left";

  map[5].actionid = 5;
  map[5].device = 0;
  map[5].name = "mouse_wheel_up";

  // Print validation codes
  for(i=0; i<NUM_MAPS; i++) {
    printf("--- validating map %i: ", i);
    e = oi_action_validate(&(map[i]));
    printf("code %i\n", e);
  }

  // Install the map
  e = oi_action_install(map, NUM_MAPS);
  printf("--- installed map, code %i\n", e);
  if(e != OI_ERR_OK) {
    printf("--- map install failed - aborting\n");
    return;
  }
  else {
    printf("--- map install ok!\n");
  }

  // Try to fetch an event
  e = 1;
  while(e) {
    oi_events_wait(&ev);

    // Quit
    if(ev.type == OI_QUIT) {
      printf("--- quit event - aborting\n");
      e = 0;
    }

    if(ev.type == OI_ACTION) {
      printf("--- action event\n");
      printf("device: %hhu\nactionid: %u\nstate: %hhu\n",
             ev.action.device, ev.action.actionid, ev.action.state);
      printf("data1: %i\ndata2: %i\ndata3: %i\n\n",
             ev.action.data1, ev.action.data2, ev.action.data3);
    }
  }
}

/* ******************************************************************** */

// Main function
int main(int argc, char* argv[]) {
  Display *disp;
  Window win;
  XEvent evt;
  unsigned int scrn;
  long event_mask;
  char csw[100];
  int i;

  // Startup
  printf("*** x11actiontest start ***\n\n");

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

  // Init OI
  sprintf(csw, "c:%lu s:%u w:%u", (ulong)disp, (uint)scrn, (uint)win);
  i = oi_init(csw, 0);
  printf("--- oi_init, code %i\n\n", i);

  // Perform the test
  if(i == OI_ERR_OK) {
    test();
  }
  else {
    printf("--- openinput not initialized - test not performed\n");
  }

  // Close OI
  i = oi_close();
  printf("--- oi_close, code %i\n\n", i);

  // Close window and connection
  XDestroyWindow(disp, win);
  XCloseDisplay(disp);

  printf("*** x11actiontest stop ***\n\n");

  return 0;
}
