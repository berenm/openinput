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
#include <stdarg.h>

// Turn on/off debugging
#define PRN_EVT 0
#define PRN_KEYUP 1
#define PRN_KEYDOWN 1
#define PRN_MOUSEMOVE 0
#define PRN_MOUSEBUT 0
#define PRN_JOYSTICK 1
#define PRN_DISCEXT 1
#define PRN_DISC 1
#define PRN_GRABHIDE 0
#define PRN_HELP 1
#define PRN_WINDOW 0
#define PRN_UNKNOWN 0

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

// Condiditional printing
void plprn(int print, char *format, ...) {
    va_list args;

    // The condition for printing
    if(!print) {
        return;
    }

    fprintf(stderr, "platform: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

/* ******************************************************************** */

// Show some help
static void help() {
    printf("platform: platform independent test help\n");
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
    plprn(PRN_DISCEXT, "discover index:%u name:'%s' desc:'%s' provides:0x%X",
          index, name, desc, pro);
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

        plprn(PRN_EVT, "got event");

        // Quit
        if(ev.type == OI_QUIT) {
            e = 0;
        }

        // Joystick axis
        else if(ev.type == OI_JOYAXIS) {
            plprn(PRN_JOYSTICK, "joystick axis axis:%i abs:%i rel:%i",
                   OI_JOY_DECODE_INDEX(ev.joyaxis.code), ev.joyaxis.abs, ev.joyaxis.rel);
        }

        // Joystick button up
        else if(ev.type == OI_JOYBUTTONUP) {
            plprn(PRN_JOYSTICK, "joystick button up id:%i",
                   OI_JOY_DECODE_INDEX(ev.joybutton.code));
        }

        // Joystick button down
        else if(ev.type == OI_JOYBUTTONDOWN) {
            plprn(PRN_JOYSTICK, "joystick button down id:%i",
                   OI_JOY_DECODE_INDEX(ev.joybutton.code)       );
        }


        // Mouse button up
        else if((ev.type == OI_MOUSEBUTTONUP) ||
                (ev.type == OI_MOUSEBUTTONDOWN)) {
            plprn(PRN_MOUSEBUT, "mouse button state %i at position %i,%i",
                  ev.button.state, ev.button.x, ev.button.y);
        }

        // Mouse move
        else if(ev.type == OI_MOUSEMOVE) {
            plprn(PRN_MOUSEMOVE, "mouse move  abs:%i,%i \t rel:%i,%i",
                  ev.move.x, ev.move.y, ev.move.relx, ev.move.rely);
        }

        // Key down
        else if(ev.type == OI_KEYDOWN) {
            plprn(PRN_KEYDOWN, "key pressed -> %i:'%s'", ev.key.keysym.sym,
                  oi_key_getname(ev.key.keysym.sym));
        }

        // Key up
        else if(ev.type == OI_KEYUP) {
            plprn(PRN_KEYUP, "key release -> %i:'%s'", ev.key.keysym.sym,
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
                plprn(PRN_GRABHIDE, "grab state %i", sgrab);
                break;

            case OIK_C:
                // Cursor
                toggle(&scursor);
                if(scursor) {
                    oi_app_cursor(OI_ENABLE);
                } else {
                    oi_app_cursor(OI_DISABLE);
                }
                plprn(PRN_GRABHIDE, "cursor state %i", scursor);
                break;

            case OIK_M:
                // Print mouse state
                {
                    int x,y;
                    oi_mouse_absolute(0, &x, &y);
                    plprn(PRN_HELP, "mouse absolute: %i,%i\n", x, y);
                    oi_mouse_relative(0, &x, &y);
                    plprn(PRN_HELP, "mouse relative: %i,%i\n", x, y);
                }
                break;

            case OIK_W:
                // Warp
                oi_mouse_warp(0, 10, 10);
                plprn(PRN_HELP, "warped");
                break;

            case OIK_Q:
                // Quit
                e = 0;
                plprn(PRN_HELP, "quit");
                break;

            case OIK_E:
                // Enable/disable device index 2
                toggle(&senable);
                oi_device_enable(2, senable);
                printf("device 2 state %i", senable);
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
                plprn(PRN_WINDOW, "active event");
                break;

            case OI_RESIZE:
                plprn(PRN_WINDOW, "resize event");
                break;

            case OI_EXPOSE:
                plprn(PRN_WINDOW, "expose event");
                break;

            case OI_DISCOVERY:
                plprn(PRN_DISC, "discovery event");
                discovery(ev.discover.device);
                break;

            default:
                plprn(PRN_UNKNOWN, "unhandled event type %i", ev.type);
                break;
            }
        }
    }
}

/* ******************************************************************** */
