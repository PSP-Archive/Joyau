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

#include "StdInclude.hpp"
#include "Sprite.hpp"
#include "Audio.hpp"
#include "Graphics.hpp"
#include "Keys.hpp"
#include "Kernel.hpp"
#include "MessageBox.hpp"
#include "Scrolling.hpp"
#include "Particles.hpp"
#include "GameMap.hpp"
#include "CircleMenu.hpp"
#include "Timer.hpp"
#include "Drawable.hpp"
#include "DrawableRect.hpp"
#include "Circle.hpp"
#include "Line.hpp"
#include "Triangle.hpp"
#include "DrawableText.hpp"
#include "Shape.hpp"
#include "Usb.hpp"

PSP_MODULE_INFO("Joyau", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-1024);

int exit_callback(int arg1, int arg2, void* commons)
{
   sceKernelExitGame();
   return 0;
}

int CallbackThread(SceSize args, void *argp)
{
   int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
   sceKernelRegisterExitCallback(cbid);
   sceKernelSleepThreadCB();
   return 0;
}

int SetupCallbacks(void)
{
   int thid = sceKernelCreateThread("update_thread", CallbackThread,
                                    0x11, 0xFA0, THREAD_ATTR_USER, 0);
   if(thid >= 0)
      sceKernelStartThread(thid, 0, 0);
   return thid;
}

VALUE debug(VALUE self, VALUE text)
{
   oslDebug(StringValuePtr(text));
   return Qnil;
}

VALUE Joyau_puts(VALUE self, VALUE text)
{
   pspDebugScreenPrintf(StringValuePtr(text));
   return Qnil;
}

int main(int argc, char** argv)
{
   string scriptName = "script.rb";
   string scriptFilename = argv[0];
   size_t id = scriptFilename.rfind('/') + 1;
   scriptFilename.erase(id);
   scriptFilename = scriptFilename + scriptName;

   pspDebugScreenInit();
   SetupCallbacks();

   ruby_init();

   defineManager();
   defineDrawable();
   defineShape();
   defineManager();
   defineSprite();
   defineAudio();
   defineGraphics();
   defineKeys();
   defineKernel();
   defineMessageBox();
   defineScrolling();
   defineParticles();
   defineGameMap();
   defineCircleMenu();
   defineTimer();
   defineDrawableRect();
   defineCircle();
   defineLine();
   defineTriangle();
   defineDrawableText();
   defineUsb();

   Manager::getInstance().setArg(argc, argv);

   defFunc("debug", debug, 1);
   defFunc("puts", Joyau_puts, 1); // puts redefined for the psp
   
   ruby_init_loadpath();
   ruby_script("embedded");

   rb_load_file(scriptFilename.c_str());
   ruby_run();

   Manager::deleteInstance();
   return 0;
}

