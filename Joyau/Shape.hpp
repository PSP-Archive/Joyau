/*Copyright (C) 2009 Verhetsel Kilian

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/

#ifndef JOYAU_SHAPE
#define JOYAU_SHAPE

#include "Drawable.hpp"

class Shape: public Drawable
{
public:
   Shape(int size = 1);

   void setColor(OSL_COLOR col);
   void setGradient(OSL_COLOR *col);

   OSL_COLOR *getColors() { return _col; }
   OSL_COLOR getColor() { return _col[0]; }

   int getColorsNumber() { return _size; }
protected:
   /*
     A shape can just be colorized.
    */
   OSL_COLOR *_col;
   int _size;
};

VALUE Shape_setColor(int argc, VALUE *argv, VALUE self);
VALUE Shape_setGradient(VALUE self, VALUE col);

VALUE Shape_getColorsNumber(VALUE self);
VALUE Shape_getColors(VALUE self);
VALUE Shape_getColor(VALUE self);

void defineShape();

#endif