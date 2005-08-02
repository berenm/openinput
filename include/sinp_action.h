/*
 * sinp_action.h : Definitions for action mapper interface
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

#ifndef _SINP_ACTION_H_
#define _SINP_ACTION_H_

#ifndef _SINP_H_
#error Do not include this file directly - use sinp.h
#endif

/* ********************************************************************
 * Action mapper directions and map structure
 ******************************************************************** */

// Action map structure
typedef struct sinp_actionmap {
  uint actionid;                 // Unique user-defined id
  char *name;                    // Trigger event (key_*, mouse_*, etc.)
} sinp_actionmap;

/* ******************************************************************** */

#endif
