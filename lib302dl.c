/*    Copyright (C) 2009 Evgeny Grablyk <evgeny.grablyk@gmail.com>

      This file is part of lib302dl.
      
      lib302dl is free software: you can redistribute it and/or modify
      it under the terms of the GNU Lesser General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.
      
      Foobar is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.
      
      You should have received a copy of the GNU Lesser General Public License
      along with lib302dl.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdio.h>
#include <avr/io.h>
#include "compat.c"
#include "defines.h"

uint8_t fullscale = 0;

/* write register */
void
lis_rwrite (uint8_t reg, uint8_t value)
{
  twi_start (LIS_WRITE_ADDR);
  twi_write (reg);
  twi_write (value);
  twi_stop ();
}

int8_t
lis_rread (uint8_t reg)
{
  int8_t recv;

  twi_start (LIS_WRITE_ADDR);
  twi_write (reg);
  twi_stop ();
  twi_start (LIS_READ_ADDR);
  recv = twi_read ();
  twi_stop ();

  return recv;
}

uint8_t
lis_initialize (uint8_t high_datarate, uint8_t dopowerup,
		uint8_t setfullscale)
{
  if (lis_rread (LIS_WHOAMI) != LIS_WHOAMI_VALUE)
    return 1;

  if (dopowerup)
    lis_rwrite (LIS_CR1, _BV (LIS_PD));
  if (high_datarate)
    lis_rwrite (LIS_CR1, _BV (LIS_DR));
  if (setfullscale)
    {
      fullscale = 1;
      lis_rwrite (LIS_CR1, _BV (LIS_FS));
    }      

  return 0;
}

int8_t
lis_rx (void)
{
  return lis_rread (LIS_OX);
}

int8_t
lis_ry (void)
{
  return lis_rread (LIS_OY);
}

int8_t
lis_rz (void)
{
  return lis_rread (LIS_OZ);
}

int16_t
lis_rxa (void)
{
  if (fullscale)
    return lis_rx() * LIS_SENSIVITY_FS1;
  else
    return lis_rx() * LIS_SENSIVITY_FS0;
}

int16_t
lis_rya (void)
{
  if (fullscale)
    return lis_ry() * LIS_SENSIVITY_FS1;
  else
    return lis_ry() * LIS_SENSIVITY_FS0;
}

int16_t
lis_rza (void)
{
  if (fullscale)
    return lis_rz() * LIS_SENSIVITY_FS1;
  else
    return lis_rz() * LIS_SENSIVITY_FS0;
}

