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

#ifndef JOYAU_CIRCLE_MENU
#define JOYAU_CIRCLE_MENU

#include "Drawable.hpp"

typedef struct {
   int AlphaMenu;
   int AngleVoullue;
   int nbIcones;
   int SelectionEncour;
   OSL_IMAGE *Image;
   int TailleIconeX;
   int TailleIconeY;
   int Eloignement;
   int test_angle;
   int PossitionX;
   int PossitionY;
   int LargeurAnimX;
   int LargeurAnimY;
} _OSL_CIRCLE_MENU;

/** @addtogroup Drawables **/
/*@{*/

/**
 * @class CircleMenu
 * A class which allows to draw a menu,
 * whose icons are disposed as in a circle.
 */
class CircleMenu: public Drawable
{
public:
   CircleMenu();
   CircleMenu(const CircleMenu &obj);
   virtual ~CircleMenu();
   
   /** Loads a menu.
    *  @param str menu's filename.
    *  @param nbIcons Tells how many icons there are on the menu.
    *  @param dist The menu's distance.
    *  @param w menu's width
    *  @param h menu's height
    *  @param x x position of the menu.
    *  @param y y position of the menu.
    *  @param iconW icon's width in the file.
    *  @param iconH icon's height in the file.
    */
   void load(char *str, int nbIcons, int dist, int w, int h, int x, int y,
	     int iconW, int iconH);
   
   /** Changes the selected item.
    *  @param dir 1 for the left direction, 0 for the right direction.
    */
   void changeSelection(int dir);

   /** Returns the menu index **/
   int getIndex();

   void draw();

   /** Sets the menu alpha
    *  @param alpha new alpha value.
    */
   void setAlpha(int alpha);

   /** Sets the menu distance
    *  @param dist new dist value.
    */
   void setDist(int dist);

   /** Sets the menu angle
    *  @param angle new angle value.
    */
   void setAngle(int angle);

   /** Returns the menu alpha **/
   int getAlpha() const { return _menu->AlphaMenu; }

   /** Returns the menu dist **/
   int getDist() const { return _menu->Eloignement; }

   /** Returns the menu angle **/
   int getAngle() const { return _menu->AngleVoullue; }
private:
   _OSL_CIRCLE_MENU *_menu;
};

/*@}*/

VALUE CircleMenu_load(VALUE self, VALUE str, VALUE nbIcons, VALUE dist, VALUE w, 
		      VALUE h, VALUE x, VALUE y, VALUE iconW, VALUE iconH);

VALUE CircleMenu_changeSelection(VALUE self, VALUE dir);
VALUE CircleMenu_getIndex(VALUE self);

VALUE CircleMenu_setAlpha(VALUE self, VALUE alpha);
VALUE CircleMenu_setDist(VALUE self, VALUE dist);
VALUE CircleMenu_setAngle(VALUE self, VALUE angle);

VALUE CircleMenu_alpha(VALUE self);
VALUE CircleMenu_dist(VALUE self);
VALUE CircleMenu_angle(VALUE self); 

void defineCircleMenu();

#endif
