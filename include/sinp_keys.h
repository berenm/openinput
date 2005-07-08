/*
 * sinp_keys.h : Basic key and modifier definitions
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

#ifndef _SINP_KEYS_H_
#define _SINP_KEYS_H_

#ifndef _SINP_H_
#error Do not include this file directly - use sinp.h
#endif

/* ******************************************************************** */

// Key definitions
typedef enum {
  SK_FIRST              = 0,
  SK_FOO                = 1,
  SK_LAST
} sinp_key;
  
/* ******************************************************************** */

// Modifier definitions
typedef enum {
  SM_NONE               = 0x0000,
  SM_LSHIFT             = 0x0001,
  SM_RSHIFT             = 0x0002,
  SM_RESERVER           = 0x8000
} sinp_mod;

/* ******************************************************************** */

// Keyboard virtual symbol
typedef struct {
  uchar scancode;       // Hardware scancode
  sinp_key sym;         // Key symbol
  sinp_mod mod;         // Modifier symbol
  ushort unicode;       // Translated unicode character
} sinp_keysym;

/* ******************************************************************** */

#endif
