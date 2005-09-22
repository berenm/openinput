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

// Platform independent test function
void platform_test();

/* ******************************************************************** */

// Main X11 function
int main(int argc, char* argv[]) {
    Display *disp;
    Window win;
    XEvent evt;
    unsigned int scrn;
    long event_mask;
    char csw[100];
    int e;

    // Startup
    printf("x11test: start\n");

    // Open connection
    disp = XOpenDisplay( NULL );
    if(!disp) {
        printf("x11test: could not open display\n");
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
    sprintf(csw, "c:%u s:%u w:%u", (uint)disp, (uint)scrn, (uint)win);
    printf("x11test: init parameter '%s'\n", csw);
    e = oi_init(csw, 0);
    printf("x11test: oi_init, code %i\n", e);

    // Perform the test
    platform_test();

    // Close OI
    e = oi_close();
    printf("x11test: oi_close, code %i\n", e);

    // Close window and connection
    XDestroyWindow(disp, win);
    XCloseDisplay(disp);
    printf("x11test: stop\n");

    return 0;
}

/* ******************************************************************** */
