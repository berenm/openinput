/*
 * openinput_mouse.h : Definitions for mouse interface
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

#ifndef _OPENINPUT_MOUSE_H_
#define _OPENINPUT_MOUSE_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ******************************************************************** */

/**
 * @ingroup PTypes
 * @defgroup PMousename Mouse button definitions
 * @brief Mouse buttons
 *
 * Definition of mouse buttons
 */
typedef enum {
  OIP_UNKNOWN            = 0,
  OIP_FIRST              = 0,
  OIP_BUTTON_LEFT        = 1,
  OIP_BUTTON_MIDDLE      = 2,
  OIP_BUTTON_RIGHT       = 3,
  OIP_WHEEL_UP           = 4,
  OIP_WHEEL_DOWN         = 5,
  OIP_MOTION             = 6,
  OIP_LAST
} oi_mouse;


/**
 * @ingroup PTypes
 * @defgroup PMouseMask Mouse button masks
 * @brief Definition of mouse button masks
 *
 * Mouse button bits used in state parameters.
 * @{
 */
#define OI_BUTTON_MASK(x) (1<<(x))
#define OI_BUTTON_LEFTMASK      OI_BUTTON_MASK(OIP_BUTTON_LEFT)
#define OI_BUTTON_MIDMASK       OI_BUTTON_MASK(OIP_BUTTON_MIDDLE)
#define OI_BUTTON_RIGHTMASK     OI_BUTTON_MASK(OIP_BUTTON_RIGHT)
/** @} */

/* ******************************************************************** */

#endif
