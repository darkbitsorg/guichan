/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/sdl/sdlinput.hpp"

#include "guichan/exception.hpp"

namespace gcn
{
    SDLInput::SDLInput()
    {
        mMouseInWindow = true;
        mMouseDown = false;
    }

    bool SDLInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput SDLInput::dequeueKeyInput()
    {
        KeyInput keyInput;

        if (mKeyInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    bool SDLInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput SDLInput::dequeueMouseInput()
    {
        MouseInput mouseInput;

        if (mMouseInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void SDLInput::pushInput(SDL_Event event)
    {
        KeyInput keyInput;
        MouseInput mouseInput;

        switch (event.type)
        {
          case SDL_KEYDOWN:
          {
              int value = convertSDLEventToGuichanKeyValue(event);

              if (value == -1)
              {
                  value = (int)event.key.keysym.unicode;
              } 
               
              keyInput.setKey(Key(value));
              keyInput.setType(KeyInput::Pressed);
              keyInput.setShiftPressed(event.key.keysym.mod & KMOD_SHIFT);
              keyInput.setControlPressed(event.key.keysym.mod & KMOD_CTRL);
              keyInput.setAltPressed(event.key.keysym.mod & KMOD_ALT);
              keyInput.setMetaPressed(event.key.keysym.mod & KMOD_META);
              keyInput.setNumericPad(event.key.keysym.sym >= SDLK_KP0
                                     && event.key.keysym.sym <= SDLK_KP_EQUALS);

              mKeyInputQueue.push(keyInput);
              break;
          }

          case SDL_KEYUP:
          {
              int value = convertSDLEventToGuichanKeyValue(event);

              if (value == -1)
              {
                  value = (int)event.key.keysym.sym;
              } 

              keyInput.setKey(Key(value));
              keyInput.setType(KeyInput::Released);
              keyInput.setShiftPressed(event.key.keysym.mod & KMOD_SHIFT);
              keyInput.setControlPressed(event.key.keysym.mod & KMOD_CTRL);
              keyInput.setAltPressed(event.key.keysym.mod & KMOD_ALT);
              keyInput.setMetaPressed(event.key.keysym.mod & KMOD_META);
              keyInput.setNumericPad(event.key.keysym.sym >= SDLK_KP0
                                     && event.key.keysym.sym <= SDLK_KP_EQUALS);

              mKeyInputQueue.push(keyInput);
              break;
          }

          case SDL_MOUSEBUTTONDOWN:
              mMouseDown = true;
              mouseInput.setX(event.button.x);
              mouseInput.setY(event.button.y);
              mouseInput.setButton(convertMouseButton(event.button.button));

              if (event.button.button == SDL_BUTTON_WHEELDOWN)
              {
                  mouseInput.setType(MouseInput::WheelMovedDown);
              }
              else if (event.button.button == SDL_BUTTON_WHEELUP)
              {
                  mouseInput.setType(MouseInput::WheelMovedUp);
              }
              else
              {
                  mouseInput.setType(MouseInput::Pressed);
              }
              mouseInput.setTimeStamp(SDL_GetTicks());
              mMouseInputQueue.push(mouseInput);
              break;

          case SDL_MOUSEBUTTONUP:
              mMouseDown = false;
              mouseInput.setX(event.button.x);
              mouseInput.setY(event.button.y);
              mouseInput.setButton(convertMouseButton(event.button.button));
              mouseInput.setType(MouseInput::Released);
              mouseInput.setTimeStamp(SDL_GetTicks());
              mMouseInputQueue.push(mouseInput);
              break;

          case SDL_MOUSEMOTION:
              mouseInput.setX(event.button.x);
              mouseInput.setY(event.button.y);
              mouseInput.setButton(MouseInput::Empty);
              mouseInput.setType(MouseInput::Moved);
              mouseInput.setTimeStamp(SDL_GetTicks());
              mMouseInputQueue.push(mouseInput);
              break;

          case SDL_ACTIVEEVENT:
              /*
               * This occurs when the mouse leaves the window and the Gui-chan
               * application loses its mousefocus.
               */
              if ((event.active.state & SDL_APPMOUSEFOCUS)
                  && !event.active.gain)
              {
                  mMouseInWindow = false;

                  if (!mMouseDown)
                  {
                      mouseInput.setX(-1);
                      mouseInput.setY(-1);
                      mouseInput.setButton(MouseInput::Empty);
                      mouseInput.setType(MouseInput::Moved);
                      mMouseInputQueue.push(mouseInput);
                  }
              }

              if ((event.active.state & SDL_APPMOUSEFOCUS)
                  && event.active.gain)
              {
                  mMouseInWindow = true;
              }
              break;

        } // end switch
    }

    int SDLInput::convertMouseButton(int button)
    {
        switch (button)
        {
          case SDL_BUTTON_LEFT:
              return MouseInput::Left;
              break;
          case SDL_BUTTON_RIGHT:
              return MouseInput::Right;
              break;
          case SDL_BUTTON_MIDDLE:
              return MouseInput::Middle;
              break;
          default:
              // We have an unknown mouse type which is ignored.
              return button;
        }
    }

    int SDLInput::convertSDLEventToGuichanKeyValue(SDL_Event event)
    {
        int value = -1;

        switch (event.key.keysym.sym)
        {
          case SDLK_TAB:
              value = Key::Tab;
              break;
          case SDLK_LALT:
              value = Key::LeftAlt;
              break;
          case SDLK_RALT:
              value = Key::RightAlt;
              break;
          case SDLK_LSHIFT:
              value = Key::LeftShift;
              break;
          case SDLK_RSHIFT:
              value = Key::RightShift;
              break;
          case SDLK_LCTRL:
              value = Key::LeftControl;
              break;
          case SDLK_RCTRL:
              value = Key::RightControl;
              break;
          case SDLK_BACKSPACE:
              value = Key::Backspace;
              break;
          case SDLK_PAUSE:
              value = Key::Pause;
              break;
          case SDLK_SPACE:
              // Special characters like ~ (tilde) ends up
              // with the keysym.sym SDLK_SPACE which
              // without this check would be lost. The check
              // is only valid on key down events in SDL.
              if (event.type == SDL_KEYUP || event.key.keysym.unicode == ' ')
              {
                  value = Key::Space;
              }
              break;
          case SDLK_ESCAPE:
              value = Key::Escape;
              break;
          case SDLK_DELETE:
              value = Key::Delete;
              break;
          case SDLK_INSERT:
              value = Key::Insert;
              break;
          case SDLK_HOME:
              value = Key::Home;
              break;
          case SDLK_END:
              value = Key::End;
              break;
          case SDLK_PAGEUP:
              value = Key::PageUp;
              break;
          case SDLK_PRINT:
              value = Key::PrintScreen;
              break;
          case SDLK_PAGEDOWN:
              value = Key::PageDown;
              break;
          case SDLK_F1:
              value = Key::F1;
              break;
          case SDLK_F2:
              value = Key::F2;
              break;
          case SDLK_F3:
              value = Key::F3;
              break;
          case SDLK_F4:
              value = Key::F4;
              break;
          case SDLK_F5:
              value = Key::F5;
              break;
          case SDLK_F6:
              value = Key::F6;
              break;
          case SDLK_F7:
              value = Key::F7;
              break;
          case SDLK_F8:
              value = Key::F8;
              break;
          case SDLK_F9:
              value = Key::F9;
              break;
          case SDLK_F10:
              value = Key::F10;
              break;
          case SDLK_F11:
              value = Key::F11;
              break;
          case SDLK_F12:
              value = Key::F12;
              break;
          case SDLK_F13:
              value = Key::F13;
              break;
          case SDLK_F14:
              value = Key::F14;
              break;
          case SDLK_F15:
              value = Key::F15;
              break;
          case SDLK_NUMLOCK:
              value = Key::NumLock;
              break;
          case SDLK_CAPSLOCK:
              value = Key::CapsLock;
              break;
          case SDLK_SCROLLOCK:
              value = Key::ScrollLock;
              break;
          case SDLK_RMETA:
              value = Key::RightMeta;
              break;
          case SDLK_LMETA:
              value = Key::LeftMeta;
              break;
          case SDLK_LSUPER:
              value = Key::LeftSuper;
              break;
          case SDLK_RSUPER:
              value = Key::RightSuper;
              break;
          case SDLK_MODE:
              value = Key::AltGr;
              break;
          case SDLK_UP:
              value = Key::Up;
              break;
          case SDLK_DOWN:
              value = Key::Down;
              break;
          case SDLK_LEFT:
              value = Key::Left;
              break;
          case SDLK_RIGHT:
              value = Key::Right;
              break;
          case SDLK_RETURN:
              value = Key::Enter;
              break;
          case SDLK_KP_ENTER:
              value = Key::Enter;
              break;

          default:
              break;
        }

        if (!(event.key.keysym.mod & KMOD_NUM))
        {
            switch (event.key.keysym.sym)
            {
              case SDLK_KP0:
                  value = Key::Insert;
                  break;
              case SDLK_KP1:
                  value = Key::End;
                  break;
              case SDLK_KP2:
                  value = Key::Down;
                  break;
              case SDLK_KP3:
                  value = Key::PageDown;
                  break;
              case SDLK_KP4:
                  value = Key::Left;
                  break;
              case SDLK_KP5:
                  value = 0;
                  break;
              case SDLK_KP6:
                  value = Key::Right;
                  break;
              case SDLK_KP7:
                  value = Key::Home;
                  break;
              case SDLK_KP8:
                  value = Key::Up;
                  break;
              case SDLK_KP9:
                  value = Key::PageUp;
                  break;
              default:
                  break;
            }
        }

        return value;
    }
}
