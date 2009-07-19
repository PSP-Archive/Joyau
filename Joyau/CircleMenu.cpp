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

#include "CircleMenu.hpp"

CircleMenu::CircleMenu()
{
   _menu = NULL;
}

CircleMenu::~CircleMenu()
{
   if (_menu != NULL)
      oslDeleteCircleMenu(_menu);
}

void CircleMenu::load(char *str, int nbIcons, int dist, int w, int h, 
		      int x, int y, int iconW, int iconH)
{
   _menu = oslCreateCircleMenu(str, nbIcons, dist, 0, w, h, x, y, iconW, iconH);
}

void CircleMenu::changeSelection(int dir)
{
   oslMenuMoveIcons(_menu, dir);
}

int CircleMenu::getIndex()
{
   // SelectionEncour should be the menu index.
   return _menu->SelectionEncour;
}

void CircleMenu::draw()
{
   oslMenuUpdateIcons(_menu);
}

void CircleMenu::setAlpha(int alpha)
{
   _menu->AlphaMenu = alpha;
}

void CircleMenu::move(int x, int y)
{
   _menu->X += x;
   _menu->Y += y;
}

void CircleMenu::setPos(int x, int y)
{
   _menu->X = x;
   _menu->Y = y;
}

void CircleMenu::setDist(int dist)
{
   _menu->Eloignement = dist;
}

void CircleMenu::setAngle(int angle)
{
   _menu->AngleVoullue = angle;
}

VALUE CircleMenu_load(VALUE self, VALUE str, VALUE nbIcons, VALUE dist, VALUE w, 
		      VALUE h, VALUE x, VALUE y, VALUE iconW, VALUE iconH)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   
   char *_str = StringValuePtr(str);
   int _nbIcons = FIX2INT(nbIcons);
   int _dist = FIX2INT(dist);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);
   int _iconW = FIX2INT(iconW);
   int _iconH = FIX2INT(iconH);

   ref.load(_str, _nbIcons, _dist, _w, _h, _x, _y, _iconW, _iconH);
   return Qnil;
}

VALUE CircleMenu_changeSelection(VALUE self, VALUE dir)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _dir = FIX2INT(dir);

   ref.changeSelection(_dir);
   return Qnil;
}

VALUE CircleMenu_getIndex(VALUE self)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int ret = ref.getIndex();

   return INT2FIX(ret);
}

VALUE CircleMenu_draw(VALUE self)
{
   CircleMenu &ref = getRef<CircleMenu>(self);

   ref.draw();
   return Qnil;
}

VALUE CircleMenu_setAlpha(VALUE self, VALUE alpha)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _alpha = FIX2INT(alpha);

   ref.setAlpha(_alpha);
   return Qnil;
}

VALUE CircleMenu_move(VALUE self, VALUE x, VALUE y)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.move(_x, _y);
   return Qnil;
}

VALUE CircleMenu_setPos(VALUE self, VALUE x, VALUE y)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setPos(_x, _y);
   return Qnil;
}

VALUE CircleMenu_setDist(VALUE self, VALUE dist)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _dist = FIX2INT(dist);
   
   ref.setDist(_dist);
   return Qnil;
}

VALUE CircleMenu_setAngle(VALUE self, VALUE angle)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _angle = FIX2INT(angle);
   
   ref.setDist(_angle);
   return Qnil;
}

void defineCircleMenu()
{
   VALUE cCircleMenu = defClass<CircleMenu>("CircleMenu");
   defMethod(cCircleMenu, "load", CircleMenu_load, 9);
   defMethod(cCircleMenu, "changeSelection", CircleMenu_changeSelection, 1);
   defMethod(cCircleMenu, "getIndex", CircleMenu_getIndex, 0);
   defMethod(cCircleMenu, "draw", CircleMenu_draw, 0);
   defMethod(cCircleMenu, "setAlpha", CircleMenu_setAlpha, 1);
   defMethod(cCircleMenu, "move", CircleMenu_move, 2);
   defMethod(cCircleMenu, "setPos", CircleMenu_setPos, 2);
   defMethod(cCircleMenu, "setDist", CircleMenu_setDist, 1);
   defMethod(cCircleMenu, "setAngle", CircleMenu_setAngle, 1);
}