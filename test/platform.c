/*
 * platform.c : Platform independent test driver
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
#include <stdio.h>

/* ******************************************************************** */

// Bit toggler
static void toggle(int *i) {
  if(*i) {
    *i = 0;
  }
  else {
    *i = 1;
  }
}

/* ******************************************************************** */

// Show some help
static void help() {
  printf("platform: x11test help\n");
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
static void discovery(unsigned char index) {
  char *name;
  char *desc;
  unsigned int pro;

  // Fetch info
  oi_device_info(index, &name, &desc, &pro);
  printf("platform: device info for device index %u\n", index);
  printf("    name:     '%s'\n", name);
  printf("    desc:     '%s'\n", desc);
  printf("    provides: '0x%X'\n", pro);
}

/* ******************************************************************** */

// The openinput platform independent test
void platform_test() {
  int e;
  oi_event ev;
  int sgrab;
  int scursor;
  int senable;

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

#ifdef INSANE
    printf("platform: got event\n");
#endif

    // Quit
    if(ev.type == OI_QUIT) {
      e = 0;
    }

    // Mouse button up
    else if((ev.type == OI_MOUSEBUTTONUP) ||
            (ev.type == OI_MOUSEBUTTONDOWN)) {
      printf("platform: mouse button state %i at position %i,%i\n",
             ev.button.state, ev.button.x, ev.button.y);
    }

    // Mouse move
    else if(ev.type == OI_MOUSEMOVE) {
      printf("platform: mouse move  abs:%i,%i \t rel:%i,%i\n",
             ev.move.x, ev.move.y,
             ev.move.relx, ev.move.rely);
    }

    // Key down
    else if(ev.type == OI_KEYDOWN) {
      printf("platform: key pressed -> %i:'%s'\n", ev.key.keysym.sym,
             oi_key_getname(ev.key.keysym.sym));
    }

    // Key up
    else if(ev.type == OI_KEYUP) {
      printf("platform: key release -> %i:'%s'\n", ev.key.keysym.sym,
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
        printf("platform: grab state %i\n", sgrab);
        break;

      case OIK_C:
        // Cursor
        toggle(&scursor);
        if(scursor) {
          oi_app_cursor(OI_ENABLE);
        } else {
          oi_app_cursor(OI_DISABLE);
        }
        printf("platform: cursor state %i\n", scursor);
        break;

      case OIK_M:
        // Print mouse state
        {
          int x,y;
          oi_mouse_absolute(0, &x, &y);
          printf("platform: mouse absolute: %i,%i\n", x, y);
          oi_mouse_relative(0, &x, &y);
          printf("platform: mouse relative: %i,%i\n", x, y);
        }
        break;

      case OIK_W:
        // Warp
        oi_mouse_warp(0, 10, 10);
        printf("platform: warped\n");
        break;

      case OIK_Q:
        // Quit
        e = 0;
        printf("platform: quit!\n");
        break;

      case OIK_E:
        // Enable/disable device index 2
        toggle(&senable);
        oi_device_enable(2, senable);
        printf("platform: device 2 state %i\n", senable);
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
        printf("platform: active event\n");
        break;

      case OI_RESIZE:
        printf("platform: resize event\n");
        break;

      case OI_EXPOSE:
        printf("platform: expose event\n");
        break;

      case OI_DISCOVERY:
        printf("platform: discovery event\n");
        discovery(ev.discover.device);
        break;

      default:
        printf("platform: unhandled event type %i\n", ev.type);
        break;
      }
    }
  }
}

/* ******************************************************************** */
