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

#include "Font.hpp"
#include "Manager.hpp"

void IntraText::load(const std::string &name, int options)
{
   font = Manager::getInstance().getIntraFont(name, options);
   if (font == NULL)
      throw RubyException(rb_eRuntimeError, "the font could not be loaded.");
}

int IntraText::getW() const
{
   return maxWidth == -1 ? intraFontMeasureText(font, txt.c_str()) : maxWidth;
}

void IntraText::activate()
{
   intraFontActivate(font);
}

void IntraText::setStyle(float size, int color, int shadowColor, int option)
{
   scale   = size;
   _col    = color;
   _shadow = shadowColor;
   _style  = option;
}

void IntraText::setEncoding(int options)
{
   _encoding = options;
}

void IntraText::setAltFont(IntraText &val)
{
   intraFontSetAltFont(font, val.font);
}

void IntraText::draw()
{
   intraFontSetEncoding(font, _encoding);
   intraFontSetStyle(font, scale, _col, _shadow, _style);
   if (maxWidth == -1)
      intraFontPrint(font, getX(), getY(), txt.c_str());
   else
      intraFontPrintColumn(font, getX(), getY(), maxWidth, txt.c_str());
}

/*
  Inits the intrafont module.
*/
VALUE Intrafont_init(VALUE self)
{
   intraFontInit();
   return Qnil;
}

/*
  Stops the intrafont module.
*/
VALUE Intrafont_stop(VALUE self)
{
   intraFontShutdown();
   return Qnil;
}

/*
  call-seq: text=(val)

  Sets an intratext's content.
*/
VALUE IntraText_setText(VALUE self, VALUE txt)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.setText(StringValuePtr(txt));

   return txt;
}

/*
  Returns an intratext's content.
*/
VALUE IntraText_text(VALUE self)
{
   IntraText &ref = getRef<IntraText>(self);
   return rb_str_new2(ref.getText().c_str());
}

/*
  call-seq: max_width=(val)

  Sets an intratext's maximum width.
*/
VALUE IntraText_setMaxWidth(VALUE self, VALUE val) {
   IntraText &ref = getRef<IntraText>(self);
   ref.setMaxWidth(FIX2INT(val));

   return val;
}

/*
  call-seq: load(font_name, options)

  Loads a font. You can use flags as option, or give 0.
*/
VALUE IntraText_load(VALUE self, VALUE name, VALUE options)
{
   IntraText &ref = getRef<IntraText>(self);
   try {
      ref.load(StringValuePtr(name), FIX2INT(options));
   }
   catch (const RubyException &e) {
      e.rbRaise();
   }

   return Qnil;
}

/*
  Activates a font.
*/
VALUE IntraText_activate(VALUE self)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.activate();

   return Qnil;
}

/*
  call-seq: setStyle(size, color, shadow, options)

  Sets a font style. Flags of options can be given too.
*/
VALUE IntraText_setStyle(VALUE self, VALUE size, VALUE color, VALUE shadow, 
			 VALUE options)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.setStyle(NUM2DBL(size), hash2col(color), hash2col(shadow), 
		FIX2INT(options));

   return Qnil;
}

/*
  call-seq: encoding=(options)

  Sets the text's encoding (required for non-ASCII texts).
*/
VALUE IntraText_setEncoding(VALUE self, VALUE options)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.setEncoding(FIX2INT(options));

   return Qnil;
}

/*
  call-seq: altFont=(font)

  Sets the font from which characters will be taken 
  if it is not in this one.
*/
VALUE IntraText_setAltFont(VALUE self, VALUE obj)
{
   IntraText &ref = getRef<IntraText>(self);
   IntraText &val = getRef<IntraText>(obj);
   
   ref.setAltFont(val);
   return Qnil;
}

/*
  Document-class: Joyau::Intrafont
  
  Intrafont is used to load fonts present in the PSP's flash.
  Constant defined here can be used when loading them.
*/

/*
  Document-class: Joyau::Intratext

  Drawable used when drawing text through Intrafont.
*/
void defineIntrafont()
{
   VALUE mIntraFont = defModule("Intrafont");
   defModFunc(mIntraFont, "init", Intrafont_init, 0);
   defModFunc(mIntraFont, "stop", Intrafont_stop, 0);

   defConst(mIntraFont, "ADVANCE_H", INT2FIX(INTRAFONT_ADVANCE_H));
   defConst(mIntraFont, "ADVANCE_V", INT2FIX(INTRAFONT_ADVANCE_V));

   defConst(mIntraFont, "ALIGN_LEFT", INT2FIX(INTRAFONT_ALIGN_LEFT));
   defConst(mIntraFont, "ALIGN_CENTER", INT2FIX(INTRAFONT_ALIGN_CENTER));
   defConst(mIntraFont, "ALIGN_RIGHT", INT2FIX(INTRAFONT_ALIGN_RIGHT));
   defConst(mIntraFont, "ALIGN_FULL", INT2FIX(INTRAFONT_ALIGN_FULL));

   defConst(mIntraFont, "SCROLL_LEFT", INT2FIX(INTRAFONT_SCROLL_LEFT));
   defConst(mIntraFont, "SCROLL_SEESAW", INT2FIX(INTRAFONT_SCROLL_SEESAW));
   defConst(mIntraFont, "SCROLL_RIGHT", INT2FIX(INTRAFONT_SCROLL_RIGHT));
   defConst(mIntraFont, "SCROLL_THROUGH", INT2FIX(INTRAFONT_SCROLL_THROUGH));

   defConst(mIntraFont, "WIDTH_VAR", INT2FIX(INTRAFONT_WIDTH_VAR));
   defConst(mIntraFont, "WIDTH_FIX", INT2FIX(INTRAFONT_WIDTH_FIX));

   defConst(mIntraFont, "ACTIVE", INT2FIX(INTRAFONT_ACTIVE));

   defConst(mIntraFont, "CACHE_MED", INT2FIX(INTRAFONT_CACHE_MED));
   defConst(mIntraFont, "CACHE_LARGE", INT2FIX(INTRAFONT_CACHE_LARGE));
   defConst(mIntraFont, "CACHE_ASCII", INT2FIX(INTRAFONT_CACHE_ASCII));
   defConst(mIntraFont, "CACHE_ALL", INT2FIX(INTRAFONT_CACHE_ALL));

   defConst(mIntraFont, "STRING_ASCII", INT2FIX(INTRAFONT_STRING_ASCII));
   defConst(mIntraFont, "STRING_CP437", INT2FIX(INTRAFONT_STRING_CP437));
   defConst(mIntraFont, "STRING_CP850", INT2FIX(INTRAFONT_STRING_CP850));
   defConst(mIntraFont, "STRING_CP866", INT2FIX(INTRAFONT_STRING_CP866));
   defConst(mIntraFont, "STRING_SJIS", INT2FIX(INTRAFONT_STRING_SJIS));
   defConst(mIntraFont, "STRING_GBK", INT2FIX(INTRAFONT_STRING_GBK));
   defConst(mIntraFont, "STRING_KOR", INT2FIX(INTRAFONT_STRING_KOR));
   defConst(mIntraFont, "STRING_BIG5", INT2FIX(INTRAFONT_STRING_BIG5));
   defConst(mIntraFont, "STRING_CP1251", INT2FIX(INTRAFONT_STRING_CP1251));
   defConst(mIntraFont, "STRING_CP1252", INT2FIX(INTRAFONT_STRING_CP1252));
   defConst(mIntraFont, "STRING_UTF8", INT2FIX(INTRAFONT_STRING_UTF8));
   
   VALUE cIntraText = defClass<IntraText>("IntraText", "Drawable");
   defMethod(cIntraText, "text=", IntraText_setText, 1);
   defMethod(cIntraText, "text", IntraText_text, 0);
   defMethod(cIntraText, "max_width=", IntraText_setMaxWidth, 1);
   defMethod(cIntraText, "load", IntraText_load, 2);
   defMethod(cIntraText, "activate", IntraText_activate, 0);
   defMethod(cIntraText, "setStyle", IntraText_setStyle, 4);
   defMethod(cIntraText, "encoding=", IntraText_setEncoding, 1);
   defMethod(cIntraText, "altFont=", IntraText_setAltFont, 1);
}
