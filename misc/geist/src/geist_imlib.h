/* geist_imlib.h

Copyright (C) 1999,2000 Tom Gilbert.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies of the Software and its documentation and acknowledgment shall be
given in the documentation and software packages that this Software was
used.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <Imlib2.h>
#include <stdarg.h>
#include "geist.h"

#ifndef GEIST_IMLIB_H
#define GEIST_IMLIB_H

#define CLIP(x, y, w, h, xx, yy, ww, hh) \
{ \
   if ((yy) > y) { h -= (yy) - y; y = (yy); } \
   if ((yy) + hh < y + h) { h -= y + h - ((yy) + (hh)); } \
   if ((xx) > x) { w -= (xx) - x; x = (xx); } \
   if ((xx) + (ww) < x + w) { w -= x + w - ((xx) + (ww)); } \
}

int geist_imlib_load_image(Imlib_Image * im, char *filename);
DATA8 geist_imlib_image_part_is_transparent(Imlib_Image im, int x, int y);

#endif
