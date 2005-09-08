/*
 * joystick.c : Generic joystick interface
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
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openinput.h"
#include "internal.h"
#include "private.h"

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Initialize joystick state manager
 *
 * @returns errorcode, see @ref PErrors
 *
 * Called on library initialization. The function prepares
 * the joystick manager for use.
 */
sint joystick_init() {
  debug("joystick_init");

  // Done
  return OI_ERR_OK;
}


/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Allocate and prepare private managment data
 *
 * @param joy pointer to the address of the joystick data
 * @param provide provide mask, see @ref PProvide
 *
 * @pre This function is called during "device_register" where
 * the base device->managment structure has been allocated and
 * initialized
 *
 * This function (possible) allocates and initializes the joystick
 * per-device private managment data, ie. axes positions and button
 * states
 *
 * The structure might also not be created, depending on whether
 * the device provides joystick as determined by the provide-mask
 */
void joystick_manage(oi_privjoy **joy, uint provide) {
  // Only care about keyboard
  if(!(provide & OI_PRO_JOYSTICK)) {
    return;
  }
  
  // Allocate
  *joy = (oi_privjoy*)malloc(sizeof(oi_privjoy));

  // Clear states
  (*joy)->button = 0;
  memset((*joy)->relaxes, 0, TABLESIZE((*joy)->relaxes));
  memset((*joy)->absaxes, 0, TABLESIZE((*joy)->absaxes));
  memset((*joy)->update, 0, TABLESIZE((*joy)->update));

  debug("joystick_manage: manager data installed");
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Joystick axis update
 *
 * @param index device index
 * @param axis axis index
 * @param value value (position/motion) of axis
 * @param post true (1) to post event, false (0) otherwise
 *
 * Send a joystick axis update into the library. This function
 * will handle the decoding of the axis index and determine
 * whether the axis is a stick-axes, hat, throttle, rudder
 * or trackball-axes.
 *
 * Note that this function does not inject events into the queue, but merely
 * tags exes for "update needed". This allows for the post-event generation
 * "joystick_pump" to combine multi-axes things (balls, sticks, etc.) into
 * a single event.
 */
void joystick_axis(uchar index, uchar axis, sint value, uchar post) {
  sint corval;
  oi_privjoy *priv;
  oi_joyconfig *conf;

  // Get private data or bail
  priv = device_priv(index, OI_PRO_JOYSTICK);
  if(!priv) {
    debug("joystick_axis: not a joystick");
    return;
  }
  
  // Get joystick config
  conf = device_get(index)->joyconfig;
  if(!conf) {
    debug("joystick_axis: no joystick config");
    return;
  }

  // Clip value
  if(value > OI_JOY_AXIS_MAX) {
    corval = OI_JOY_AXIS_MAX;
  }
  else if(value < OI_JOY_AXIS_MIN) {
    corval = OI_JOY_AXIS_MIN;
  }
  else {
    corval = value;
  }

  // Store state
  priv->relaxes[axis] += corval - priv->relaxes[axis];
  priv->absaxes[axis] = corval;

  // Flag update
  priv->update[axis] |= post;
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Joystick button update
 *
 * @param index device index
 * @param btn button index
 * @param state pressed (true) or released (false)
 * @param post true (1) to post event, false (0) otherwise
 *
 * Feed joystick button press/release into joystick state manager.
 */
void joystick_button(uchar index, uchar btn, uchar state, uchar post) {
  uint newbut;
  uchar type;
  oi_privjoy *priv;

  // Get private data or bail
  priv = device_priv(index, OI_PRO_JOYSTICK);
  if(!priv) {
    return;
  }

  // Calculate button mask
  newbut = priv->button;
  if(state) {
    type = OI_JOYBUTTONUP;
    newbut |= OI_BUTTON_MASK(btn);
  }
  else {
    type = OI_JOYBUTTONDOWN;
    newbut &= ~OI_BUTTON_MASK(btn);
  }
  priv->button = newbut;

  // Postal services
  if(post) {
    oi_event ev;
    ev.type = type;
    ev.joybutton.device = index;
    ev.joybutton.button = btn;
    ev.joybutton.state = newbut;
    queue_add(&ev);
  }
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Pump pending joystick events into queue
 *
 * Here, joystick axes with the 'update' flag set are analyzed,
 * possibly paired, and sent to the event queue. We need to do this as
 * the last step in the joystick frame in order to collect x- and
 * y-axes for trackballs, hats etc.
 */
void joystick_pump() {
  uchar index;
  oi_device *dev;
  oi_privjoy *priv;
  oi_joyconfig *conf;
  oi_event ev;
  uchar axis;
  sshort val;

  // Parse all devices
  for(index=1; index<OI_MAX_DEVICES; index++) {
    // Devices are always ordered, so bail if we hit the end
    if(!(dev = device_get(index))) {
      break;
    }

    // Get structures
    conf = dev->joyconfig;
    priv = device_priv(index, OI_PRO_JOYSTICK);

    // Only handle joysticks with valid thingies
    if(!(dev->provides & OI_PRO_JOYSTICK) || !conf || !priv) {
      continue;
    }

    // Ok, we have a valid joystick, parse each axis
    for(axis=0; axis<OI_JOY_NUM_AXES; axis++) {

      // Bail if no-axis or no update needed
      if((conf->kind[axis] == OI_AXIS_NONE) || !(priv->update[axis])) {
	continue;
      }

      // Trackballs have special event
      if(conf->kind[axis] == OI_AXIS_BALL) {

	// Get vertical movement
	if(conf->pair[axis] != 0) {
	  val = priv->relaxes[conf->pair[axis]];
	}
	else {
	  val = 0;
	}

	// Send trackball event
	ev.type = OI_JOYBALL;
	ev.joyball.device = index;
	ev.joyball.ball = axis;
	ev.joyball.relx = priv->relaxes[axis];
	ev.joyball.rely = val;
	queue_add(&ev);
	
	// We're done with this axis
	continue;	
      }

      // Default axis value
      val = priv->absaxes[axis];
      
      // Handle pairing for hats
      if((conf->pair[axis] != 0) && (conf->kind[axis] == OI_AXIS_HAT)) {
	val = joystick_hatpos(priv->absaxes[axis],
			      priv->absaxes[conf->pair[axis]]);
      }

      // Send absolute axis event
      ev.type = OI_JOYAXIS;
      ev.joyaxis.device = index;
      ev.joyaxis.axis = axis;
      ev.joyaxis.kind = conf->kind[axis];
      ev.joyaxis.value = val;
      queue_add(&ev);
    }    
  }
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Hat position converter
 *
 * @param x horizontal axis
 * @param y vertival axis
 * @returns hat posistion, see @ref PJoyHats
 *
 * Under some OSes (like Linux), joystick hats are reported using
 * multiple axes.  This function converts two-axis hats to the
 * discrete joystick hat position values found as OI_HAT_*.
 */
sint joystick_hatpos(sint x, sint y) {
  const sint hatpos[3][3] = {
    {OI_HAT_UPLEFT, OI_HAT_UP, OI_HAT_UPRIGHT},
    {OI_HAT_LEFT, OI_HAT_CENTER, OI_HAT_RIGHT},
    {OI_HAT_DOWNLEFT, OI_HAT_DOWN, OI_HAT_DOWNRIGHT} };
  uchar cx;
  uchar cy;

  // Clip
  if(x < 0) {
    cx = 0;
  }
  else if(x == 0) {
    cx = 1;
  }
  else {
    cx = 2;
  }

  if(y < 0) {
    cy = 0;
  }
  else if(y == 0) {
    cy = 1;
  }
  else {
    cy = 2;
  }

  // Ok, now it's just a lookup
  return hatpos[cy][cx];
}

/* ******************************************************************** */
