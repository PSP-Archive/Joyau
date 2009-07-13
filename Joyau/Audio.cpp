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

#include "Audio.hpp"

Sound::~Sound()
{
   alDeleteBuffers(1, &buffer);
   alDeleteSources(1, &source);
}

bool Sound::loadWav(const char *filename)
{
   alGenBuffers(1, &buffer);
   if (alGetError() != AL_NO_ERROR)
      return false;
   buffer = alutCreateBufferFromFile(filename);

   // We create a source, binded to our buffer :
   alGenSources(1, &source);
   if (alGetError() != AL_NO_ERROR)
      return false;

   alSourcei(source, AL_BUFFER, buffer);

   alSource3f(source, AL_POSITION, 0.f, 0.f, 0.f);
   alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f);
   alSource3f(source, AL_DIRECTION, 0.f, 0.f, 0.f);
   
   if (alGetError() == AL_NO_ERROR)
      return true;
   return false;
}

void Sound::play()
{
   ALint state;
   alGetSourcei(source, AL_SOURCE_STATE, &state);
   if (state != AL_PLAYING)
      alSourcePlay(source);
}

void Sound::pause()
{
   alSourcePause(source);
}

void Sound::stop()
{
   alSourceStop(source);
}

void Sound::setPos(float x, float y, float z)
{
   alSource3f(source, AL_POSITION, x, y, z);
}

void Sound::setVelocity(float x, float y, float z)
{
   alSource3f(source, AL_VELOCITY, x, y, z);
}

void Sound::setDirection(float x, float y, float z)
{
   alSource3f(source, AL_DIRECTION, x, y, z);
}

Stream::~Stream()
{
   alSourceStop(source);
   clear();

   alDeleteSources(1, &source);
   alDeleteBuffers(1, buffers);

   ov_clear(&stream);
}

void Stream::setPos(float x, float y, float z)
{
   alSource3f(source, AL_POSITION, x, y, z);
}

void Stream::setVelocity(float x, float y, float z)
{
   alSource3f(source, AL_VELOCITY, x, y, z);
}

void Stream::setDirection(float x, float y, float z)
{
   alSource3f(source, AL_DIRECTION, x, y, z);
}

bool Stream::loadOgg(const char *filename)
{
   file = fopen(filename, "rb");
   ov_open(file, &stream, NULL, 0);

   info = ov_info(&stream, -1);
   comment = ov_comment(&stream, -1);

   format = SAMPLES_FORMAT;

   alGenBuffers(2, buffers);
   if (alGetError() != AL_NO_ERROR)
      return false;

   alGenSources(1, &source);
   if (alGetError() != AL_NO_ERROR)
      return false;

   alSource3f(source, AL_POSITION, 0.f, 0.f, 0.f);
   alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f);
   alSource3f(source, AL_DIRECTION, 0.f, 0.f, 0.f);

   alSourcef(source, AL_ROLLOFF_FACTOR, 0.f);
   alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);

   return true;
}

bool Stream::play()
{
   // We don't want to play this Stream twice, do we ?
   if(playing())
      return true;
   if(!streamBuf(buffers[0]))
      return false;   
   if(!streamBuf(buffers[1]))
      return false;

   alSourceQueueBuffers(source, 2, buffers);
   alSourcePlay(source);
   
   return true;
}

void Stream::pause()
{
   alSourceQueueBuffers(source, 2, buffers);
   alSourcePause(source);
}

void Stream::stop()
{
   alSourceQueueBuffers(source, 2, buffers);
   alSourceStop(source);
}

bool Stream::playing()
{
   ALenum state;
   alGetSourcei(source, AL_SOURCE_STATE, &state);

   return state == AL_PLAYING;
}

bool Stream::update()
{
   int processed;
   alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
   
   bool active = true;
   while (processed--)
   {
      ALuint buffer;
      alSourceUnqueueBuffers(source, 1, &buffer);
      active = streamBuf(buffer);
      
      alSourceQueueBuffers(source, 1, &buffer);
   }
   return active;
}

bool Stream::streamBuf(ALuint buffer)
{
   char data[BUFFER_SIZE];
   
   int size = 0;
   int section, result;

   while (size < BUFFER_SIZE)
   {
      result = ov_read(&stream, data + size, BUFFER_SIZE - size, 
		       0, 2, 1, &section);
      if (result > 0)
	 size += result;
      else
      {
	 if (result < 0)
	    return false;
	 else
	    break;
      }
   }

   if (size == 0)
      return false;

   alBufferData(buffer, format, data, size, SAMPLES_RATE);
   if (alGetError() != AL_NO_ERROR)
      return false;

   return true;
}

void Stream::clear()
{
   int queued;
   alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

   while (queued--)
   {
      ALuint buffer;
      alSourceUnqueueBuffers(source, 1, &buffer);
   }
}

void initOpenAl()
{
   int argc = Manager::getInstance()->getArgc();
   char **argv = Manager::getInstance()->getArgv();
   alutInit(&argc, argv);
}

void stopOpenAL()
{
   alutExit();
}

VALUE Audio_init(VALUE self)
{
   initOpenAl();
   return Qnil;
}

VALUE Audio_stop(VALUE self)
{
   stopOpenAL();
   return Qnil;
}

VALUE Sound_loadWav(VALUE self, VALUE filename)
{
   Sound *ptr = getPtr<Sound>(self);
   char *str = StringValuePtr(filename);

   if (ptr->loadWav(str))
      return Qtrue;
   return Qfalse;
}

VALUE Sound_play(VALUE self)
{
   Sound *ptr = getPtr<Sound>(self);
   ptr->play();
   return Qnil;
}

VALUE Sound_pause(VALUE self)
{
   Sound *ptr = getPtr<Sound>(self);
   ptr->pause();

   return Qnil;
}

VALUE Sound_stop(VALUE self)
{
   Sound *ptr = getPtr<Sound>(self);
   ptr->stop();

   return Qnil;
}

VALUE Sound_setPos(VALUE self, VALUE x, VALUE y, VALUE z)
{
   Sound *ptr = getPtr<Sound>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ptr->setPos((float)_x, (float)_y, (float)_z);

   return Qnil;
}

VALUE Sound_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z)
{
   Sound *ptr = getPtr<Sound>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ptr->setVelocity((float)_x, (float)_y, (float)_z);

   return Qnil;
}

VALUE Sound_setDirection(VALUE self, VALUE x, VALUE y, VALUE z)
{
   Sound *ptr = getPtr<Sound>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ptr->setDirection((float)_x, (float)_y, (float)_z);

   return Qnil;
}

VALUE Stream_loadOgg(VALUE self, VALUE filename)
{
   Stream *ptr = getPtr<Stream>(self);
   char *str = StringValuePtr(filename);

   if (ptr->loadOgg(str))
      return Qtrue;
   return Qfalse;
}

VALUE Stream_play(VALUE self)
{
   Stream *ptr = getPtr<Stream>(self);
   if (ptr->play())
      return Qtrue;
   return Qfalse;
}

VALUE Stream_pause(VALUE self)
{
   Stream *ptr = getPtr<Stream>(self);
   ptr->pause();

   return Qnil;
}

VALUE Stream_stop(VALUE self)
{
   Stream *ptr = getPtr<Stream>(self);
   ptr->stop();

   return Qnil;
}

VALUE Stream_playing(VALUE self)
{
   Stream *ptr = getPtr<Stream>(self);
   if (ptr->playing())
      return Qtrue;
   return Qfalse;
}

VALUE Stream_update(VALUE self)
{
   Stream *ptr = getPtr<Stream>(self);
   if (ptr->update())
      return Qtrue;
   return Qfalse;
}

VALUE Stream_setPos(VALUE self, VALUE x, VALUE y, VALUE z)
{
   Stream *ptr = getPtr<Stream>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ptr->setPos((float)_x, (float)_y, (float)_z);

   return Qnil;
}

VALUE Stream_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z)
{
   Stream *ptr = getPtr<Stream>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ptr->setVelocity((float)_x, (float)_y, (float)_z);

   return Qnil;
}

VALUE Stream_setDirection(VALUE self, VALUE x, VALUE y, VALUE z)
{
   Stream *ptr = getPtr<Stream>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ptr->setDirection((float)_x, (float)_y, (float)_z);

   return Qnil;
}

void defineAudio()
{
   VALUE cSound = defClass<Sound>("Sound");
   defMethod(cSound, "loadWav", Sound_loadWav, 1);
   defMethod(cSound, "play", Sound_play, 0);
   defMethod(cSound, "pause", Sound_play, 0);
   defMethod(cSound, "stop", Sound_stop, 0);
   defMethod(cSound, "setPos", Sound_setPos, 3);
   defMethod(cSound, "setDirection", Sound_setDirection, 3);
   defMethod(cSound, "setVelocity", Sound_setVelocity, 3);

   VALUE cStream = defClass<Stream>("Stream");
   defMethod(cStream, "loadOgg", Stream_loadOgg, 1);
   defMethod(cStream, "play", Stream_play, 0);
   defMethod(cStream, "playing", Stream_playing, 0);
   defMethod(cStream, "update", Stream_update, 0);
   defMethod(cStream, "pause", Stream_play, 0);
   defMethod(cStream, "stop", Stream_stop, 0);
   defMethod(cStream, "setPos", Stream_setPos, 3);
   defMethod(cStream, "setDirection", Stream_setDirection, 3);
   defMethod(cStream, "setVelocity", Stream_setVelocity, 3);

   defFunc("initAudio", Audio_init, 0);
   defFunc("stopAudio", Audio_stop, 0);
}
