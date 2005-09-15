/*
 * keynametest.c : Test key code/names
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
#include <string.h>
#include "openinput.h"

/* ******************************************************************** */

// Main function
int main(int argc, char *argv[]) {
  oi_key k;
  oi_key c;
  char *name;
  int i;
  int j;
  int m;

  // Init openinput
  printf("*** keynametest start\n");
  oi_init("c:0 s:0 w:0", OI_FLAG_NOWINDOW);
  printf("*** openinput initialized\n");

  printf("test: '%s'\n", oi_key_getname(OIK_A));

  // Run through all keys
  for(k=OIK_FIRST; k<OIK_LAST; k++) {

    // Convert to name and back again
    name = oi_key_getname(k);
    c = oi_key_getcode(name);

    // Pretty-printing
    printf("Key '%3i'\t getname: '%s'", k, name);
    for(i=strlen(name); i<20; i++) {
      printf(" ");
    }
    printf("\t getkey: '%i'", c);

    // Does keycode -> name -> keycode match?
    if(k != c) {
      printf("\t fail\n");
    }
    else {
      printf("\t good\n");
    }      
  }

  // Run through all mouse buttons
  for(j=0; j<8; j++) {
    
    // Convert to name and back again
    name = oi_mouse_getname(j);
    m = oi_mouse_getcode(name);

    // Pretty-printing
    printf("Mouse '%3i'\t getname: '%s'", j, name);
    for(i=strlen(name); i<20; i++) {
      printf(" ");
    }
    printf("\t getkey: '%i'", m);

    // Does keycode -> name -> keycode match?
    if(j != m) {
      printf("\t fail\n");
    }
    else {
      printf("\t good\n");
    }      
      }
  
  // Close openinput
  printf("*** keynametest end\n");
  oi_close();
  return 0;
}

/* ******************************************************************** */
