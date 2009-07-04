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

#ifndef __SPRITE__
#define __SPRITE__

#include "Manager.hpp"
#include "RubyWrapper.hpp"

class Sprite
{
public:
   Sprite(Manager *a_manager = Manager::getInstance()):
      _alpha(255),
      _zoom(160),
      _angle(0)
   {
      manager = a_manager;
      _x = 0;
      _y = 0;

      animated = false;
      
      _nbrX = 1;
      _nbrY = 1;

      passedTime = 0;
      animeState = 0;
      nbrAnime = 6;

      tiled = false;
      xTile = 0;
      yTile = 0;
      wTile = 0;
      hTile = 0;
   }

   void setPicture(char *pic);

   void setPos(int x, int y);
   void move(int x, int y);

   void rotate(int angle) { _angle += angle; }
   int getAngle() { return _angle; }
   void setAngle(int angle) { _angle = angle; }
   void zoom(int increase);
   int getZoom() { return _zoom; }

   void setAlpha(int alpha);
   int getAlpha();
   
   int getX();
   int getY();
   int getW();
   int getH();

   bool isOn(int x, int y);

   bool collide(Sprite *spr);

   virtual void Draw();
   
   enum DIRECTION { DOWN, LEFT, RIGHT, UP, UP_LEFT, UP_RIGHT, DOWN_LEFT,
		    DOWN_RIGHT };

   void setDirection(int dir);
   int getDirection() { return _dir; }
   
   // Configure the number of sprite.
   void setAnimation(int nbrX, int nbrY);
   void setAnimationTime(int t) { nbrAnime = t; }

   void setTile(int x, int y, int w, int h);

   // This function would set the correct position, ...
   // before returning the pointer
   // ( It's safe because the sprite change these value when the picture is
   // drawn )
   OSL_IMAGE *getImage();

   void saveImage(const char *fname);

protected:
   string picName;
   Manager *manager;

   void defaultDraw();
private:
   int _x, _y;
   int _w, _h;
   int _stretchX, _stretchY;
   int _alpha;
   int _zoom;
   int _angle;
   int _dir;

   OSL_IMAGE *sprite;
   
   bool animated;
   int _nbrX;
   int _nbrY;
   int animeState;
   int passedTime;
   int nbrAnime;

   bool tiled;
   int xTile;
   int yTile;
   int wTile;
   int hTile;
};

enum DIRECTION { DOWN, LEFT, RIGHT, UP, UP_LEFT, UP_RIGHT, DOWN_LEFT,
		 DOWN_RIGHT };

VALUE Sprite_setPicture(VALUE self, VALUE pic);

VALUE Sprite_setPos(VALUE self, VALUE x, VALUE y);
VALUE Sprite_move(VALUE self, VALUE x, VALUE y);

VALUE Sprite_rotate(VALUE self, VALUE angle);
VALUE Sprite_getAngle(VALUE self);
VALUE Sprite_setAngle(VALUE self, VALUE angle);

VALUE Sprite_zoom(VALUE self,VALUE increase);
VALUE Sprite_getZoom(VALUE self);

VALUE Sprite_setAlpha(VALUE self,VALUE alpha);
VALUE Sprite_getAlpha(VALUE self);

VALUE Sprite_getX(VALUE self);
VALUE Sprite_getY(VALUE self);
VALUE Sprite_getW(VALUE self);
VALUE Sprite_getH(VALUE self);

VALUE Sprite_isOn(VALUE self, VALUE x, VALUE y);
VALUE Sprite_collide(VALUE self, VALUE spr);

VALUE Sprite_draw(VALUE self);
VALUE Sprite_saveFile(VALUE self, VALUE pic);

VALUE Sprite_setDirection(VALUE self, VALUE dir);
VALUE Sprite_getDirection(VALUE self);

VALUE Sprite_setAnimation(VALUE self, VALUE nbrX, VALUE nbrY);
VALUE Sprite_setAnimationTime(VALUE self, VALUE t);

VALUE Sprite_setTile(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h);

void defineSprite();

#endif
