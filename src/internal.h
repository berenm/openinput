/*
 * internal.h : Header for internal functions
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

#ifndef _OPENINPUT_INTERNAL_H_
#define _OPENINPUT_INTERNAL_H_

/* ******************************************************************** */

// Special functions
inline sint oi_runstate();
inline uint oi_getticks();

/* ******************************************************************** */

// Internal queue functions
sint queue_init();
inline sint queue_lock();
inline sint queue_unlock();
sint queue_cut(ushort where);
sint queue_add(oi_event *evt);
sint queue_peep(oi_event *evts, sint num, uint mask, sint remove);

/* ******************************************************************** */

// Device abstraction interface
typedef struct oi_device {
  sint index;                                                          // Device index
  char *name;                                                          // Short device name
  char *desc;                                                          // Description of device
  uint provides;                                                       // Provide-flag
  void *private;                                                       // Private data
  sint (*init)(struct oi_device *dev, char *window_id, uint flags);    // Initialize device
  sint (*destroy)(struct oi_device *dev);                              // Shutdown device
  void (*process)(struct oi_device *dev);                              // Pump events into queue
  sint (*grab)(struct oi_device *dec, sint on);                        // Grab input focus
  sint (*hide)(struct oi_device *dev, sint on);                        // Hide/show cursor
  sint (*warp)(struct oi_device *dev, sint x, sint y);                 // Warp mouse cursor
  sint (*winsize)(struct oi_device *dev, sint *w, sint *h);            // Query for window size
} oi_device;

// Platform bootstrap interface
typedef struct oi_bootstrap {
  char *name;                                                          // Short device name
  char *desc;                                                          // Device description
  sint provides;                                                       // Device provide-flag
  sint (*avail)(uint flags);                                           // Is device available?
  struct oi_device *(*create)();                                       // Return device structure
} oi_bootstrap;

/* ******************************************************************** */

// Device handling
sint device_register(struct oi_bootstrap *boot);
void device_bootstrap(uint flags);
sint device_init(sint index, char *window_id, uint flags);
oi_device *device_get(sint index);
inline void device_pumpall();
uint device_windowid(char *str, char tok);
sint device_destroy(sint index);

/* ******************************************************************** */

// Application state
sint appstate_init();
void appstate_focus(sint gain, sint state, uchar post);
inline sint appstate_width();
inline sint appstate_height();
void appstate_resize(sint w, sint h, uchar postdev);

/* ******************************************************************** */

// Mouse state
sint mouse_init();
void mouse_move(sint x, sint y, sint relative, uchar postdev);
void mouse_button(sint btn, sint state, uchar postdev);

/* ******************************************************************** */

// Keyboard state
sint keyboard_init();
sint keyboard_fillnames(char **kn);
void keyboard_update(oi_keysym *keysym, sint state, uchar postdev);
void keyboard_dorepeat();
void keyboard_setmodifier(uint newmod);
inline oi_key keyboard_scangetkey(char *name, oi_key first, oi_key last);

/* ******************************************************************** */

// Action state
sint action_init();
inline void action_process(oi_event *evt);

/* ******************************************************************** */

// Debug macro
#ifdef DEBUG
void debug(char *format, ...);
#else
#define debug(format, args...) ((void)0)
#endif

// Table size helper
#define TABLESIZE(table) (sizeof(table)/sizeof(table[0]))

// True and false
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Misc constants
#define OI_MAX_DEVICES 64
#define OI_MAX_EVENTS 128
#define OI_SLEEP 10
#define OI_MIN_KEYLENGTH 5
#define OI_MAX_KEYLENGTH 20

/* ******************************************************************** */

#endif
