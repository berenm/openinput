/*
 * sinp_mouse.h : Definitions for mouse interface
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

#ifndef _SINP_MOUSE_H_
#define _SINP_MOUSE_H_

#ifndef _SINP_H_
#error Do not include this file directly - use sinp.h
#endif

/* ********************************************************************
 * Mouse buttons and masks
 ******************************************************************** */

// The pointer buttons
typedef enum {
  SP_UNKNOWN            = 0,
  SP_FIRST              = 0,
  SP_BUTTON_LEFT        = 1,
  SP_BUTTON_MIDDLE      = 2,
  SP_BUTTON_RIGHT       = 3,
  SP_WHEEL_UP           = 4,
  SP_WHEEL_DOWN         = 5,
  SP_MOTION             = 6,
  SP_LAST               = 7
} sinp_mouse;

// Masks for buttons
#define SINP_BUTTON_MASK(x) (1<<(x))
#define SINP_BUTTON_LEFTMASK      SINP_BUTTON_MASK(SP_BUTTON_LEFT)
#define SINP_BUTTON_MIDMASK       SINP_BUTTON_MASK(SP_BUTTON_MIDDLE)
#define SINP_BUTTON_RIGHTMASK     SINP_BUTTON_MASK(SP_BUTTON_RIGHT)

/* ******************************************************************** */

#endif
