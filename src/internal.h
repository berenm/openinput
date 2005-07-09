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

// Device abstraction interface
typedef struct sinp_device {
  sint index;                                                          // Device index
  char *name;                                                          // Short device name
  char *desc;                                                          // Description of device
  uint status;                                                         // Status-flag
  uint provides;                                                       // Provide-flag
  void *private;                                                       // Private data
  sint (*init)(struct sinp_device *dev, char *window_id, uint flags);  // Initialize device
  sint (*enable)(struct sinp_device *dev, sint on);                    // Enable/disable device
  sint (*destroy)(struct sinp_device *dev);                            // Shutdown device
  void (*process)(struct sinp_device *dev);                            // Pump events into queue
  sint (*grab)(struct sinp_device *dec, uint mask);                    // Grab input "provide" mask
} sinp_device;

/* ******************************************************************** */

// Internal queue functions
sint queue_init();
sint queue_lock();
sint queue_unlock();
sint queue_cut(ushort where);

/* Important notes:
 * -> queue_add() will _copy_ the given event into the queue, ie.
 * you can throw the memory away after the call.
 * -> queue_peep() will _copy_ the event into the user's pointer,
 * ie. you must allocate space for the event(s) before calling the
 * functon
 */
sint queue_add(sinp_event *evt);
sint queue_peep(sinp_event *evts, sint num, uint mask, sint remove);

/* ******************************************************************** */

// Internal device function prototypes
void device_boot();
sinp_device *device_get(sint index);
void device_set_provides(sint index, uint pro);
void device_set_status(sint index, uint sta);
void device_pumpall();
uint device_windowid(char *str, char tok);

/* ******************************************************************** */

// Debug macro
#ifdef DEBUG
void debug(char *format, ...);
#else
#define debug(format, args...) ((void)0)
#endif

/* ******************************************************************** */

#endif
