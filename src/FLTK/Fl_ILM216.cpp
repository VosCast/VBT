//
// "$Id: Fl_ILM216.cxx,v 1.8 2003/02/26 01:59:31 easysw Exp $"
//
// ILM-216 LCD emulator widget code for flcdsim.
//
// Copyright 2003 by Michael Sweet.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Contents:
//
//   Fl_ILM216::Fl_ILM216()  - Create a new ILM-216 widget.
//   Fl_ILM216::~Fl_ILM216() - Delete an ILM-216 widget.
//   Fl_ILM216::draw()       - Draw the widget.
//   Fl_LCD216::load_char()  - Load a single character in the font.
//   Fl_LCD216::load_font()  - Load the standard ILM-216 font.
//   Fl_LCD216::process()    - Process data like an ILM-216.
//

//modified by Daniel Noethen

//
// Include necessary headers...
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <FL/fl_draw.H>

#include "butt.h"
#include "flgui.h"

#include "../xpm/rec.xpm" //rec_xpm
#include "../xpm/rec_dark.xpm" //rec_xpm
#include "../xpm/rec_armed.xpm" //rec_xpm
#include "../xpm/conn.xpm" //conn_xpm
#include "../xpm/conn_dark.xpm" //conn_xpm


//
// Font data for the LCD-216...
//
//
//

static const uchar    standard_font[224][8] =            {
              // 0x20 to 0x2f
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x04, 0x00 },
              { 0x0a, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x0a, 0x0a, 0x1f, 0x0a, 0x1f, 0x0a, 0x0a, 0x00 },
              { 0x04, 0x0f, 0x14, 0x0e, 0x05, 0x1e, 0x04, 0x00 },
              { 0x18, 0x19, 0x02, 0x04, 0x08, 0x13, 0x03, 0x00 },
              { 0x0c, 0x12, 0x14, 0x08, 0x15, 0x12, 0x0d, 0x00 },
              { 0x0c, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02, 0x00 },
              { 0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08, 0x00 },
              { 0x00, 0x04, 0x15, 0x0e, 0x15, 0x04, 0x00, 0x00 },
              { 0x00, 0x04, 0x04, 0x1f, 0x04, 0x04, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x04, 0x08 },
              { 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00 },
              { 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00 },

              // 0x30 to 0x3f
              { 0x0e, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0e, 0x00 },
              { 0x04, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x1f, 0x00 },
              { 0x0e, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1f, 0x00 },
              { 0x1f, 0x02, 0x04, 0x02, 0x01, 0x11, 0x0e, 0x00 },
              { 0x02, 0x06, 0x0a, 0x12, 0x1f, 0x02, 0x02, 0x00 },
              { 0x1f, 0x10, 0x1e, 0x01, 0x01, 0x11, 0x0e, 0x00 },
              { 0x06, 0x08, 0x10, 0x1e, 0x11, 0x11, 0x0e, 0x00 },
              { 0x1f, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08, 0x00 },
              { 0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e, 0x00 },
              { 0x0e, 0x11, 0x11, 0x0f, 0x01, 0x02, 0x0c, 0x00 },
              { 0x00, 0x0c, 0x0c, 0x00, 0x0c, 0x0c, 0x00, 0x00 },
              { 0x00, 0x0c, 0x0c, 0x00, 0x0c, 0x04, 0x08, 0x00 },
              { 0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02, 0x00 },
              { 0x00, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00 },
              { 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x00 },
              { 0x0e, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04, 0x00 },

              // 0x40 to 0x4f
              { 0x0e, 0x11, 0x01, 0x0d, 0x15, 0x15, 0x0e, 0x00 },
              { 0x0e, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x00 },
              { 0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e, 0x00 },
              { 0x0e, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0e, 0x00 },
              { 0x1c, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1c, 0x00 },
              { 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f, 0x00 },
              { 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x10, 0x00 },
              { 0x0e, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0f, 0x00 },
              { 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x00 },
              { 0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1f, 0x00 },
              { 0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0c, 0x00 },
              { 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11, 0x00 },
              { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x00 },
              { 0x11, 0x1b, 0x15, 0x15, 0x11, 0x11, 0x11, 0x00 },
              { 0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x00 },
              { 0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00 },

              // 0x50 to 0x5f
              { 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10, 0x00 },
              { 0x0e, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0d, 0x00 },
              { 0x1e, 0x11, 0x11, 0x1e, 0x14, 0x12, 0x11, 0x00 },
              { 0x0f, 0x10, 0x10, 0x0e, 0x01, 0x01, 0x1e, 0x00 },
              { 0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00 },
              { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00 },
              { 0x11, 0x11, 0x11, 0x11, 0x11, 0x0a, 0x04, 0x00 },
              { 0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0a, 0x00 },
              { 0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11, 0x00 },
              { 0x11, 0x11, 0x11, 0x0a, 0x04, 0x04, 0x04, 0x00 },
              { 0x1f, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1f, 0x00 },
              { 0x0e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0e, 0x00 },
              { 0x11, 0x0a, 0x1f, 0x04, 0x1f, 0x04, 0x04, 0x00 },
              { 0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0e, 0x00 },
              { 0x04, 0x0a, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f },

              // 0x60 to 0x6f
              { 0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x0e, 0x01, 0x0f, 0x11, 0x0f, 0x00 },
              { 0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x1e, 0x00 },
              { 0x00, 0x00, 0x0e, 0x10, 0x10, 0x11, 0x0e, 0x00 },
              { 0x01, 0x01, 0x0d, 0x13, 0x11, 0x11, 0x0f, 0x00 },
              { 0x00, 0x00, 0x0e, 0x11, 0x1f, 0x10, 0x0e, 0x00 },
              { 0x06, 0x09, 0x08, 0x1c, 0x08, 0x08, 0x08, 0x00 },
              { 0x00, 0x0f, 0x11, 0x11, 0x0f, 0x01, 0x0e, 0x00 },
              { 0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00 },
              { 0x04, 0x00, 0x0c, 0x04, 0x04, 0x04, 0x0e, 0x00 },
              { 0x02, 0x00, 0x02, 0x02, 0x02, 0x12, 0x0c, 0x00 },
              { 0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12, 0x00 },
              { 0x0c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e, 0x00 },
              { 0x00, 0x00, 0x1a, 0x15, 0x15, 0x11, 0x11, 0x00 },
              { 0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00 },
              { 0x00, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e, 0x00 },

              // 0x70 to 0x7f
              { 0x00, 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x00 },
              { 0x00, 0x0d, 0x13, 0x11, 0x0f, 0x01, 0x01, 0x00 },
              { 0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10, 0x00 },
              { 0x00, 0x00, 0x0e, 0x10, 0x0e, 0x01, 0x1e, 0x00 },
              { 0x08, 0x08, 0x1c, 0x08, 0x08, 0x09, 0x06, 0x00 },
              { 0x00, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0d, 0x00 },
              { 0x00, 0x00, 0x11, 0x11, 0x11, 0x0a, 0x04, 0x00 },
              { 0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0a, 0x00 },
              { 0x00, 0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x00 },
              { 0x00, 0x11, 0x11, 0x11, 0x0f, 0x01, 0x0e, 0x00 },
              { 0x00, 0x00, 0x1f, 0x02, 0x04, 0x08, 0x1f, 0x00 },
              { 0x02, 0x04, 0x04, 0x08, 0x04, 0x04, 0x02, 0x00 },
              { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00 },
              { 0x08, 0x04, 0x04, 0x02, 0x04, 0x04, 0x08, 0x00 },
              { 0x00, 0x04, 0x02, 0x1f, 0x02, 0x04, 0x00, 0x00 },
              { 0x00, 0x04, 0x08, 0x1f, 0x08, 0x04, 0x00, 0x00 },

              // 0x80 to 0x8f
              { 0x1c, 0x10, 0x18, 0x10, 0x12, 0x02, 0x02, 0x02 },
              { 0x1c, 0x10, 0x18, 0x16, 0x11, 0x02, 0x04, 0x07 },
              { 0x1c, 0x10, 0x18, 0x16, 0x11, 0x02, 0x01, 0x06 },
              { 0x1c, 0x10, 0x18, 0x15, 0x15, 0x07, 0x01, 0x01 },
              { 0x08, 0x14, 0x14, 0x08, 0x05, 0x06, 0x05, 0x05 },
              { 0x08, 0x14, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f },
              { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },

              // 0x90 to 0x9f
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },

              // 0xa0 to 0xaf
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x14, 0x1c },
              { 0x07, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x1c },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x04 },
              { 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00 },
              { 0x00, 0x1f, 0x01, 0x1f, 0x01, 0x02, 0x04, 0x00 },
              { 0x00, 0x00, 0x00, 0x1f, 0x01, 0x06, 0x04, 0x08 },
              { 0x00, 0x00, 0x00, 0x02, 0x04, 0x0c, 0x14, 0x04 },
              { 0x00, 0x00, 0x00, 0x04, 0x1f, 0x11, 0x01, 0x06 },
              { 0x00, 0x00, 0x00, 0x00, 0x1f, 0x04, 0x04, 0x1f },
              { 0x00, 0x00, 0x00, 0x02, 0x1f, 0x06, 0x0a, 0x12 },
              { 0x00, 0x00, 0x00, 0x08, 0x1f, 0x09, 0x0a, 0x08 },
              { 0x00, 0x00, 0x00, 0x00, 0x0e, 0x02, 0x02, 0x1f },
              { 0x00, 0x00, 0x00, 0x1e, 0x02, 0x1e, 0x02, 0x1e },
              { 0x00, 0x00, 0x00, 0x00, 0x15, 0x15, 0x01, 0x06 },

              // 0xb0 to 0xbf
              { 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00 },
              { 0x1f, 0x01, 0x05, 0x06, 0x04, 0x04, 0x08, 0x00 },
              { 0x01, 0x02, 0x04, 0x0c, 0x14, 0x04, 0x04, 0x00 },
              { 0x04, 0x1f, 0x11, 0x11, 0x01, 0x02, 0x04, 0x00 },
              { 0x00, 0x1f, 0x04, 0x04, 0x04, 0x04, 0x1f, 0x00 },
              { 0x02, 0x1f, 0x02, 0x06, 0x0a, 0x12, 0x02, 0x00 },
              { 0x08, 0x1f, 0x09, 0x09, 0x09, 0x09, 0x12, 0x00 },
              { 0x04, 0x1f, 0x04, 0x1f, 0x04, 0x04, 0x04, 0x00 },
              { 0x00, 0x0f, 0x09, 0x11, 0x01, 0x02, 0x0c, 0x00 },
              { 0x08, 0x0f, 0x12, 0x02, 0x02, 0x02, 0x04, 0x00 },
              { 0x00, 0x1f, 0x01, 0x01, 0x01, 0x01, 0x1f, 0x00 },
              { 0x0a, 0x1f, 0x0a, 0x0a, 0x02, 0x04, 0x08, 0x00 },
              { 0x00, 0x18, 0x01, 0x19, 0x01, 0x02, 0x1c, 0x00 },
              { 0x00, 0x1f, 0x01, 0x02, 0x04, 0x0a, 0x11, 0x00 },
              { 0x08, 0x1f, 0x09, 0x0a, 0x08, 0x08, 0x07, 0x00 },
              { 0x00, 0x11, 0x11, 0x09, 0x01, 0x02, 0x0c, 0x00 },

              // 0xc0 to 0xcf
              { 0x00, 0x0f, 0x09, 0x15, 0x03, 0x02, 0x0c, 0x00 },
              { 0x02, 0x1c, 0x04, 0x1f, 0x04, 0x04, 0x08, 0x00 },
              { 0x00, 0x15, 0x15, 0x15, 0x01, 0x02, 0x04, 0x00 },
              { 0x0e, 0x00, 0x1f, 0x04, 0x04, 0x04, 0x08, 0x00 },
              { 0x08, 0x08, 0x08, 0x0c, 0x0a, 0x08, 0x08, 0x00 },
              { 0x04, 0x04, 0x1f, 0x04, 0x04, 0x08, 0x10, 0x00 },
              { 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00 },
              { 0x00, 0x1f, 0x01, 0x0a, 0x04, 0x0a, 0x10, 0x00 },
              { 0x04, 0x1f, 0x02, 0x04, 0x0e, 0x15, 0x04, 0x00 },
              { 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x08, 0x00 },
              { 0x00, 0x04, 0x02, 0x11, 0x11, 0x11, 0x11, 0x00 },
              { 0x10, 0x10, 0x1f, 0x10, 0x10, 0x10, 0x0f, 0x00 },
              { 0x00, 0x1f, 0x01, 0x01, 0x01, 0x02, 0x04, 0x00 },
              { 0x00, 0x08, 0x14, 0x02, 0x01, 0x01, 0x00, 0x00 },
              { 0x04, 0x1f, 0x04, 0x04, 0x15, 0x15, 0x04, 0x00 },
              { 0x00, 0x1f, 0x01, 0x01, 0x0a, 0x04, 0x02, 0x00 },

              // 0xd0 to 0xdf
              { 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x01, 0x00 },
              { 0x00, 0x04, 0x08, 0x10, 0x11, 0x1f, 0x01, 0x00 },
              { 0x00, 0x01, 0x01, 0x0a, 0x04, 0x0a, 0x10, 0x00 },
              { 0x00, 0x1f, 0x08, 0x1f, 0x08, 0x08, 0x07, 0x00 },
              { 0x08, 0x08, 0x1f, 0x09, 0x0a, 0x08, 0x08, 0x00 },
              { 0x00, 0x0e, 0x02, 0x02, 0x02, 0x02, 0x1f, 0x00 },
              { 0x00, 0x1f, 0x01, 0x1f, 0x01, 0x01, 0x1f, 0x00 },
              { 0x0e, 0x00, 0x1f, 0x01, 0x01, 0x02, 0x04, 0x00 },
              { 0x12, 0x12, 0x12, 0x12, 0x02, 0x04, 0x08, 0x00 },
              { 0x00, 0x04, 0x14, 0x14, 0x15, 0x15, 0x16, 0x00 },
              { 0x00, 0x10, 0x10, 0x11, 0x12, 0x14, 0x18, 0x00 },
              { 0x00, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x00 },
              { 0x00, 0x1f, 0x11, 0x11, 0x01, 0x02, 0x04, 0x00 },
              { 0x00, 0x18, 0x00, 0x01, 0x01, 0x02, 0x1c, 0x00 },
              { 0x04, 0x12, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x1c, 0x14, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00 },

              // 0xe0 to 0xef
              { 0x00, 0x00, 0x09, 0x15, 0x12, 0x12, 0x0d, 0x00 },
              { 0x0a, 0x00, 0x0e, 0x01, 0x0f, 0x11, 0x0f, 0x00 },
              { 0x00, 0x00, 0x0e, 0x11, 0x1e, 0x11, 0x1e, 0x00 },
              { 0x00, 0x00, 0x0e, 0x10, 0x0c, 0x11, 0x0e, 0x00 },
              { 0x00, 0x00, 0x11, 0x11, 0x13, 0x1d, 0x10, 0x00 },
              { 0x00, 0x00, 0x0f, 0x14, 0x12, 0x11, 0x0e, 0x00 },
              { 0x00, 0x00, 0x06, 0x19, 0x11, 0x19, 0x16, 0x00 },
              { 0x00, 0x00, 0x0f, 0x11, 0x11, 0x13, 0x0d, 0x00 },
              { 0x00, 0x00, 0x07, 0x04, 0x04, 0x14, 0x08, 0x00 },
              { 0x00, 0x02, 0x1a, 0x02, 0x00, 0x00, 0x00, 0x00 },
              { 0x02, 0x00, 0x06, 0x02, 0x02, 0x02, 0x02, 0x00 },
              { 0x00, 0x14, 0x08, 0x14, 0x00, 0x00, 0x00, 0x00 },
              { 0x00, 0x04, 0x0e, 0x14, 0x15, 0x0e, 0x04, 0x00 },
              { 0x08, 0x08, 0x1c, 0x08, 0x1c, 0x08, 0x0f, 0x00 },
              { 0x0e, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00 },
              { 0x0a, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e, 0x00 },

              // 0xf0 to 0xff
              { 0x00, 0x00, 0x16, 0x19, 0x11, 0x19, 0x16, 0x00 },
              { 0x00, 0x00, 0x0d, 0x13, 0x11, 0x13, 0x0d, 0x00 },
              { 0x00, 0x0e, 0x11, 0x1f, 0x11, 0x11, 0x0e, 0x00 },
              { 0x00, 0x00, 0x00, 0x0b, 0x11, 0x1a, 0x00, 0x00 },
              { 0x00, 0x00, 0x0e, 0x11, 0x11, 0x0a, 0x1b, 0x00 },
              { 0x0a, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0d, 0x00 },
              { 0x1f, 0x80, 0x04, 0x02, 0x04, 0x08, 0x1f, 0x00 },
              { 0x00, 0x00, 0x1f, 0x0a, 0x0a, 0x0a, 0x13, 0x00 },
              { 0x1f, 0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x00 },
              { 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x0f, 0x00 },
              { 0x00, 0x01, 0x1e, 0x04, 0x1f, 0x04, 0x04, 0x00 },
              { 0x00, 0x00, 0x1f, 0x08, 0x0f, 0x09, 0x11, 0x00 },
              { 0x00, 0x00, 0x1f, 0x15, 0x1f, 0x11, 0x11, 0x00 },
              { 0x00, 0x00, 0x04, 0x00, 0x1f, 0x00, 0x04, 0x00 },
              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
              { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f }
            };


//
// 'Fl_ILM216::Fl_ILM216()' - Create a new ILM-216 widget.
//

Fl_ILM216::Fl_ILM216(int        X,        // I - X position
                     int        Y,        // I - Y position
             int        W,        // I - Width
             int        H,        // I - Height
             const char *L)        // I - Label string
  : Fl_Widget(X, Y, W, H, L)
{

  // Initialize defaults...
  backlight_ = 0;

  rec_ = new Fl_Pixmap(rec_xpm);
  rec_dark_ = new Fl_Pixmap(rec_dark_xpm);
  rec_armed_ = new Fl_Pixmap(rec_armed_xpm);
  conn_ = new Fl_Pixmap(conn_xpm);
  conn_dark_ = new Fl_Pixmap(conn_dark_xpm);

  // Load the standard font...
  load_font();

}


//
// 'Fl_ILM216::~Fl_ILM216()' - Delete an ILM-216 widget.
//

Fl_ILM216::~Fl_ILM216(void)
{
  // Unload the font data...
    for (int i = 0; i < 256; i ++)
    {
        if (font_[i])
            delete font_[i];

        if (outline_[i])
            delete outline_[i];
    }
    delete rec_;
    delete rec_dark_;
    delete rec_armed_;
    delete conn_;
    delete conn_dark_;
}

int Fl_ILM216::handle(int event)
{
    if(event == FL_RELEASE)
        do_callback();

    return 1;
}


// 'Fl_ILM216::draw()' - Draw the widget.
//

void Fl_ILM216::draw(void)
{
    int        i;                // Looping var
    int        X = x() + (w() - 16 * 24) / 2;    // X base position
    int        Y = y() + (h() - 2 * 40) / 2;    // Y base position
    Fl_Color    oc;                // Outline color

    // Draw the background...
    if (backlight_)
    {
        draw_box(box(), x(), y(), w(), h(), fl_lighter((Fl_Color)cfg.main.bg_color));
        oc = fl_color_average((Fl_Color)cfg.main.txt_color, fl_lighter((Fl_Color)cfg.main.bg_color), 0.5f);
    }
    else
    {
        draw_box(box(), x(), y(), w(), h(), (Fl_Color)cfg.main.bg_color);
        oc = fl_color_average((Fl_Color)cfg.main.txt_color, (Fl_Color)cfg.main.bg_color, 0.5f);
    }

  //draw the right line 
    fl_color((Fl_Color)cfg.main.txt_color);

    fl_line_style(FL_SOLID, 1, NULL);
    fl_line(X + 15*25-15, Y, X + 15*25-15, Y+40*2);
    fl_line_style(0);

    //draw the status symbols
    if(connected)
        conn_->draw(X + 15*25 -5, Y+8);
    else
        conn_dark_->draw(X + 15*25 -5, Y+8);


    if(recording)
        rec_->draw(X + 15*25 -7, Y+8+50);
    else if (cfg.rec.start_rec)
        rec_armed_->draw(X + 15*25 -7, Y+8+50);
    else
        rec_dark_->draw(X + 15*25 -7, Y+8+50);

  // Draw the LCD contents...
    for (i = 0; i < 16; i ++)
    {
        fl_color((Fl_Color)cfg.main.txt_color);

        if (font_[chars_[i + 0]])
            font_[chars_[i + 0]]->draw(X + i * 24, Y);
        if (font_[chars_[i + 16]])
            font_[chars_[i + 16]]->draw(X + i * 24, Y + 40);

        fl_color(oc);

        if (outline_[chars_[i + 0]])
            outline_[chars_[i + 0]]->draw(X + i * 24, Y);
        if (outline_[chars_[i + 16]])
            outline_[chars_[i + 16]]->draw(X + i * 24, Y + 40);
  }
}


//
// 'Fl_LCD216::load_char()' - Load a single character in the font.
//

void Fl_ILM216::load_char(uchar       ch,        // I - Character
                     const uchar *data)        // I - 5x8 bitmap data
{
    int    i;                    // Looping var
    uchar    *fdata;                    // Pointer to font data
    uchar    *odata;                    // Pointer to outline data

    // Delete the existing character, if any...
    if (font_[ch])
    {
        delete font_[ch];
        font_[ch] = 0;
    }

    if (outline_[ch])
    {
        //delete outline_[ch];
        outline_[ch] = 0;
    }

    // Don't need to define blank chars...
    if (data[0] == 0 && !memcmp(data, data + 1, 7))
        return;

    // Create a 20x32 bitmap from the 5x8 data, so that it looks like the
    // LCD display.
    fdata = fdata_[ch - ' '];
    odata = odata_[ch - ' '];

    memset(fdata, 0, 96);
    memset(odata, 0, 96);

    for (i = 0; i < 8; i ++, fdata += 12, odata += 12, data ++)
    {
        // Each bit in the source byte is mapped to a 3x3 "pixel" and a 4x4
        // "outline" in the LCD bitmap.
        if (*data & 0x10)
        {
            fdata[0] |= 0x07;
            fdata[3] |= 0x07;
            fdata[6] |= 0x07;

            odata[0] |= 0x08;
            odata[3] |= 0x08;
            odata[6] |= 0x08;
            odata[9] |= 0x0f;
        }

        if (*data & 0x08)
        {
            fdata[0] |= 0x70;
            fdata[3] |= 0x70;
            fdata[6] |= 0x70;

            odata[0] |= 0x80;
            odata[3] |= 0x80;
            odata[6] |= 0x80;
            odata[9] |= 0xf0;
        }

        if (*data & 0x04)
        {
            fdata[1]  |= 0x07;
            fdata[4]  |= 0x07;
            fdata[7]  |= 0x07;

            odata[1]  |= 0x08;
            odata[4]  |= 0x08;
            odata[7]  |= 0x08;
            odata[10] |= 0x0f;
        }

        if (*data & 0x02)
        {
            fdata[1]  |= 0x70;
            fdata[4]  |= 0x70;
            fdata[7]  |= 0x70;

            odata[1]  |= 0x80;
            odata[4]  |= 0x80;
            odata[7]  |= 0x80;
            odata[10] |= 0xf0;
        }

        if (*data & 0x01)
        {
            fdata[2]  |= 0x07;
            fdata[5]  |= 0x07;
            fdata[8]  |= 0x07;

            odata[2]  |= 0x08;
            odata[5]  |= 0x08;
            odata[8]  |= 0x08;
            odata[11] |= 0x0f;
        }
    }

    // Create the bitmap image for this char...
    font_[ch]    = new Fl_Bitmap(fdata_[ch - ' '], 20, 32);
    outline_[ch] = new Fl_Bitmap(odata_[ch - ' '], 20, 32);
}


//
// 'Fl_LCD216::load_font()' - Load the standard ILM-216 font.
//

void Fl_ILM216::load_font(void)
{
    // Clear the font list...
    memset(font_, 0, sizeof(font_));
    memset(outline_, 0, sizeof(outline_));

    // Load standard font data for 224 chars...
    for (int i = 0; i < 224; i ++)
        load_char(i + ' ', standard_font[i]);
}

//
// 'Fl_LCD216::process()' - Process data like an ILM-216.
//

int Fl_ILM216::print(const uchar *in, int inbytes)
{

    int nbytes;

    // Loop through the input bytes...
    nbytes = 0;

    for (; inbytes > 0; inbytes --, in ++)
    {
        chars_[cursor_pos_++] = *in;

        if(*in == '\n')
            cursor_pos_ = 16;
    }

    redraw();

    // Return the number of bytes of output...
    return (nbytes);
}

//
// End of "$Id: Fl_ILM216.cxx,v 1.8 2003/02/26 01:59:31 easysw Exp $".
//
