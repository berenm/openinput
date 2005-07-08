/*
 * bootstrap.h : Header for platform bootstrap functions
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

#ifndef _SINP_BOOTSTRAP_H_
#define _SINP_BOOTSTRAP_H_

/* ******************************************************************** */

// Platform bootstrap interface
typedef struct sinp_bootstrap {
  char *name;                                // Short device name
  char *desc;                                // Device description
  sint provides;                             // Device provide-flag
  sint (*avail)();                           // Is platform valid?
  sinp_device *(*create)();                  // Return device structure
} sinp_bootstrap;

/* ******************************************************************** */

// Bootstrap forward definitions
#ifdef ENABLE_FOO
extern sinp_bootstrap foo_bootstrap;
#endif
#ifdef ENABLE_X11
extern sinp_bootstrap x11_bootstrap;
#endif

/* ******************************************************************** */

/*
 * Global variable for "device.c" only, placed here since all
 * devices needs to provide a bootstrap function and table entry
 */
#ifdef _DEVICE_FILLER_

// Bootstrap function table
static sinp_bootstrap *bootstrap[] = {
#ifdef ENABLE_FOO
  &foo_bootstrap,
#endif
#ifdef ENABLE_X11
  &x11_bootstrap,
#endif
  NULL
};

#endif

/* ******************************************************************** */

#endif
