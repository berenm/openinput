/*
 * internal.h : Header for internal functions
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

#ifndef _SINP_INTERNAL_H_
#define _SINP_INTERNAL_H_

/* ******************************************************************** */

// Special functions
inline sint sinp_runstate();

/* ******************************************************************** */

// Internal queue functions
sint queue_init();
inline sint queue_lock();
inline sint queue_unlock();
sint queue_cut(ushort where);
sint queue_add(sinp_event *evt);
sint queue_peep(sinp_event *evts, sint num, uint mask, sint remove);

/* ******************************************************************** */

// Device abstraction interface
typedef struct sinp_device {
  sint index;                                                          // Device index
  char *name;                                                          // Short device name
  char *desc;                                                          // Description of device
  uint provides;                                                       // Provide-flag
  void *private;                                                       // Private data
  sint (*init)(struct sinp_device *dev, char *window_id, uint flags);  // Initialize device
  sint (*destroy)(struct sinp_device *dev);                            // Shutdown device
  void (*process)(struct sinp_device *dev);                            // Pump events into queue
  sint (*grab)(struct sinp_device *dec, sint on);                      // Grab input focus
  sint (*hide)(struct sinp_device *dev, sint on);                      // Hide/show cursor
  sint (*warp)(struct sinp_device *dev, sint x, sint y);               // Warp mouse cursor
  sint (*winsize)(struct sinp_device *dev, sint *x, sint *y);          // Query for window size
} sinp_device;

// Platform bootstrap interface
typedef struct sinp_bootstrap {
  char *name;                                                          // Short device name
  char *desc;                                                          // Device description
  sint provides;                                                       // Device provide-flag
  sint (*avail)();                                                     // Is device available?
  struct sinp_device *(*create)();                                     // Return device structure
} sinp_bootstrap;

/* ******************************************************************** */

// Device handling
sint device_register(struct sinp_bootstrap *boot);
void device_bootstrap();
sint device_init(sint index, char *window_id, uint flags);
sinp_device *device_get(sint index);
inline void device_pumpall();
uint device_windowid(char *str, char tok);
sint device_destroy(sint index);

/* ******************************************************************** */

// Application state
sint appstate_init();
void appstate_focus(sint gain, sint state, sint post);
inline sint appstate_width();
inline sint appstate_height();

/* ******************************************************************** */

// Mouse state
sint mouse_init();
void mouse_move(sint x, sint y, sint relative, sint postdev);
void mouse_button(sint btn, sint state, sint postdev);

/* ******************************************************************** */

// Debug macro
#ifdef DEBUG
void debug(char *format, ...);
#else
#define debug(format, args...) ((void)0)
#endif

// True and false
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Misc constants
#define SINP_MAX_DEVICES 64
#define SINP_MAX_EVENTS 128
#define SINP_SLEEP 10

/* ******************************************************************** */

#endif
