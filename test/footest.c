/*
 * footest.c : Foo test driver
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
#include <stdio.h>
#include "openinput.h"

/* ******************************************************************** */

// Main function
int main(int argc, char *argv[]) {
  int e;
  oi_event ev;

  // Startup
  printf("*** footest start ***\n\n");
  
  // Init OI
  e = oi_init("c:1 s:2 w:3", 0);
  printf("--- oi_init, code %i\n\n", e);

  // Try to fetch an event
  e = oi_events_poll(&ev);
  printf("--- oi_events_poll, code %i\n", e);
  if(e > 0) {
    printf("--- event type %i\n", ev.type);
  }
  printf("\n");

  // Close OI
  e = oi_close();
  printf("--- oi_close, code %i\n\n", e);

  // Done
  printf("*** footest stop ***\n");
  return 0;
}

/* ******************************************************************** */
