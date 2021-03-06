/*Copyright (C) 2009-2010 Verhetsel Kilian

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

#include "DrawableRect.hpp"

/*
  Document-class: Joyau::DrawableRect

  Drawable used when drawing rects.
*/

/*
  call-seq: new()
            new(x, y [, x2, y2])

  Creates a new DrawableRect.
*/
VALUE DrawableRect_initialize(int argc, VALUE *argv, VALUE self)
{
   DrawableRect &ref = getRef<DrawableRect>(self);

   VALUE x1, y1, x2, y2;
   rb_scan_args(argc, argv, "04", &x1, &y1, &x2, &y2);

   if (!NIL_P(x1)  && !NIL_P(y1))
   {
      ref.setPos(FIX2INT(x1), FIX2INT(y1));
      if (argc >= 4)
	 ref.setCorner(FIX2INT(x2), FIX2INT(y2));
   }

   return Qnil;
}

DrawableRect::DrawableRect(): 
   FillableShape(4)
{
   for (int i = 0; i < 4; ++i)
      _col[i] = RGBA(255, 255, 255, 255);
   setClass("DrawableRect");
}

void DrawableRect::resize(int w, int h)
{
   _w = w;
   _h = h;

   _x2 = getX() + _w;
   _y2 = getY() + _h;
}

void DrawableRect::setW(int w)
{
   _w = w;
   _x2 = getX() + _w;
}

void DrawableRect::setH(int h)
{
   _h = h;
   _y2 = getY() + _y;
}

void DrawableRect::setCorner(int x, int y)
{
   _x2 = x;
   _y2 = y;

   _w = _x2 - getX();
   _h = _y2 - getY();
}

void DrawableRect::draw()
{
   _x2 = getX() + _w;
   _y2 = getY() + _h;

   if (filled)
      oslDrawGradientRect(getX(), getY(), _x2, _y2, 
			  _col[0], _col[1], _col[2], _col[3]);
   else
      oslDrawRect(getX(), getY(), _x2, _y2, _col[0]);
}

/*
  call-seq: resize(w, h)

  Resizes the rect.
*/
VALUE DrawableRect_resize(VALUE self, VALUE w, VALUE h)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);

   ref.resize(_w, _h);
   return Qnil;
}

/*
  call-seq: w=(width)

  Sets the rect's width.
*/
VALUE DrawableRect_setW(VALUE self, VALUE w)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   ref.setW(FIX2INT(w));

   return w;
}

/*
  call-seq: h=(height)

  Sets the rect's height.
*/
VALUE DrawableRect_setH(VALUE self, VALUE h)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   ref.setH(FIX2INT(h));

   return h;
}

/*
  call-seq: setCorner(x, y)

  Sets the rect's corner.
*/
VALUE DrawableRect_setCorner(VALUE self, VALUE x, VALUE y)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setCorner(_x, _y);
   return Qnil;
}

/*
  call-seq: corner=(point)

  Sets the rect's corner.
*/
VALUE DrawableRect_cornerOp(VALUE self, VALUE p)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   Point &point = getRef<Point>(p);
   
   ref.setCorner(point.x, point.y);
   return p;
}

/*
  Returns the rect's second corner.
*/
VALUE DrawableRect_getCorner(VALUE self)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   Point point = ref.getCorner();

   return createObject(getClass("Point"), point);
}

void defineDrawableRect()
{
   VALUE cDrawableRect = defClass<DrawableRect>("DrawableRect", "FillableShape");
   defMethod(cDrawableRect, "initialize", DrawableRect_initialize, -1);

   defMethod(cDrawableRect, "resize", DrawableRect_resize, 2);
   defMethod(cDrawableRect, "w=", DrawableRect_setW, 1);
   defMethod(cDrawableRect, "h=", DrawableRect_setH, 1);

   defMethod(cDrawableRect, "setCorner", DrawableRect_setCorner, 2);
   defMethod(cDrawableRect, "corner=", DrawableRect_cornerOp, 1); 
   defMethod(cDrawableRect, "getCorner", DrawableRect_getCorner, 0);

   defAlias(cDrawableRect, "getCorner", "corner");
}
