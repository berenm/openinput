/*
 * keynametest.c : Test key code/names
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
#include <stdio.h>
#include <string.h>
#include "sinp.h"

/* ******************************************************************** */

// Main function
int main(int argc, char *argv[]) {
  sinp_key k;
  sinp_key c;
  char *name;
  int i;
  int j;
  int m;

  // Init sinp
  printf("*** keynametest start\n");
  sinp_init("c:0 s:0 w:0", SINP_FLAG_NOWINDOW);
  printf("*** sinp initialized\n");

  // Run through all keys
  for(k=SK_FIRST; k<SK_LAST; k++) {

    // Convert to name and back again
    name = sinp_key_getname(k);
    c = sinp_key_getcode(name);

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
    name = sinp_mouse_getname(j);
    m = sinp_mouse_getcode(name);

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
  
  // Close sinp
  printf("*** keynametest end\n");
  sinp_close();
  return 0;
}

/* ******************************************************************** */
