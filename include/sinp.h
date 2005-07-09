/*
 * sinp.h : Top-level include file for libsinp
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

#ifndef _SINP_H_
#define _SINP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ******************************************************************** */

// Misc constants
#define SINP_MAX_DEVICES    64 // Max supported devices
#define SINP_MAX_EVENTS    128 // Max events in queue
#define SINP_SLEEP          10 // Milliseconds to sleep when waiting

// Init parameters for window_id
#define SINP_I_CONN        'c' // Server connection handle
#define SINP_I_SCRN        's' // Screen handle
#define SINP_I_WINID       'w' // Window handle

/* ******************************************************************** */

#include "sinp_types.h"
#include "sinp_keys.h"
#include "sinp_events.h"
#include "sinp_api.h"

/* ******************************************************************** */

#ifdef __cplusplus
}
#endif

#endif
