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

#include "Circle.hpp"

template<> VALUE wrap<Circle>(int argc, VALUE *argv, VALUE info)
{
   Circle *ptr = new Circle;
   if (argc >= 2)
   {
      ptr->setRadius(FIX2INT(argv[0]));
      
      if (argc == 2)
      	 ptr->setPos(getRef<Point>(argv[1]));
      else if (argc == 3)
	 ptr->setPos(FIX2INT(argv[1]), FIX2INT(argv[2]));
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Circle>, ptr);
   return tdata;
}

void Circle::setCenter(int x, int y)
{
   centerX = x;
   centerY = y;

   setPos(centerX - _r, centerY - _r);
   _col[0] = RGBA(255, 255, 255, 255);
}

void Circle::setRadius(int r)
{
   _r = r;
   // We'll call setPos here to, so setCenter and setRadius can be called
   // in any order.
   setPos(centerX - _r, centerY - _r);
   
   _w = _r * 2;
   _h = _r * 2;
}

void Circle::draw()
{
   // if the user has used setPos or move, we should fix centerX and centerY
   if (centerX != getX() + _r ||
       centerY != getY() + _r)
      setCenter(getX() + _r, getY() + _r);
   if (filled)
      oslDrawFillCircle(centerX, centerY, _r, _col[0]);
   else
      oslDrawCircle(centerX, centerY, _r, _col[0]);
}

VALUE Circle_setCenter(VALUE self, VALUE x, VALUE y)
{
   Circle &ref = getRef<Circle>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setCenter(_x, _y);
   return Qnil;
}

VALUE Circle_setCenterPoint(VALUE self, VALUE point)
{
   Circle &ref = getRef<Circle>(self);
   Point &p = getRef<Point>(point);
   
   ref.setCenter(p.x, p.y);
   return point;
}

VALUE Circle_setRadius(VALUE self, VALUE r)
{
   Circle &ref = getRef<Circle>(self);
   int _r = FIX2INT(r);

   ref.setRadius(_r);
   return r;
}

VALUE Circle_setColor(VALUE self, VALUE col)
{
   Circle &ref = getRef<Circle>(self);
   OSL_COLOR _col = hash2col(col);

   ref.setColor(_col);
   return Qnil;
}

VALUE Circle_getCenterX(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getCenterX());
}

VALUE Circle_getCenterY(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getCenterX());
}

VALUE Circle_center(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   Point p(ref.getCenterX(), ref.getCenterY());
   return createObject(getClass("Point"), p);
}

VALUE Circle_getRadius(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getRadius());
}

void defineCircle()
{
   VALUE cCircle = defClass<Circle>("Circle", "FillableShape");
   defMethod(cCircle, "setRadius", Circle_setRadius, 1);
   defMethod(cCircle, "setCenter", Circle_setCenter, 2);
   defMethod(cCircle, "center=", Circle_setCenterPoint, 1);
   defMethod(cCircle, "center", Circle_center, 0);
   defMethod(cCircle, "getCenterX", Circle_getCenterX, 0);
   defMethod(cCircle, "getCenterY", Circle_getCenterY, 0);
   defMethod(cCircle, "getRadius", Circle_getRadius, 0);

   defAlias(cCircle, "setRadius", "radius=");
   defAlias(cCircle, "getCenterX", "centerX");
   defAlias(cCircle, "getCenterY", "centerY");
   defAlias(cCircle, "getRadius", "radius");
}
