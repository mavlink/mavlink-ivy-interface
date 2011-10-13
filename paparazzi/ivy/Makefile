# 
#   $Id$
#   Copyright (C) 2003-2006 Pascal Brisset, Antoine Drouin
#
# This file is part of paparazzi.
#
# paparazzi is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# paparazzi is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with paparazzi; see the file COPYING.  If not, write to
# the Free Software Foundation, 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.  
# 

# Quiet
Q=@

LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  FPIC = -fPIC   
else
  FPIC =
endif

CC = gcc
GLIB_CFLAGS  = -Wall  `pkg-config glib-2.0 --cflags` $(FPIC)
GLIB_LDFLAGS =  `pkg-config glib-2.0 --libs` -lglibivy -lpcre $(FPIC)

all:
	$(CC) $(GLIB_CFLAGS) -o c_ivy_client_example c_ivy_client_example.c $(GLIB_LDFLAGS)

clean:
	rm c_ivy_client_example
