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

#include "guichan/sdl2/sdl2input.hpp"

#include "guichan/exception.hpp"

namespace gcn
{
    SDL2Input::SDL2Input()
    {
        mMouseInWindow = true;
        mMouseDown = false;
        mMouseX = 0;
        mMouseY = 0;
    }

    bool SDL2Input::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput SDL2Input::dequeueKeyInput()
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

    bool SDL2Input::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput SDL2Input::dequeueMouseInput()
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

    bool SDL2Input::hasTextInput()
    {
        return true;
    }

    bool SDL2Input::isTextQueueEmpty()
    {
        return mTextInputQueue.empty();
    }

    TextInput SDL2Input::dequeueTextInput()
    {
        TextInput textInput;

        if (mTextInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        textInput = mTextInputQueue.front();
        mTextInputQueue.pop();

        return textInput;
    }

    void SDL2Input::pushInput(SDL_Event event)
    {
        KeyInput keyInput;
        MouseInput mouseInput;

        switch (event.type)
        {
          case SDL_KEYDOWN:
          case SDL_KEYUP:
          {
              const SDL_Keysym& keysym = event.key.keysym;

              int value = convertSDLEventToGuichanKeyValue(event);

              if (value == -1)
              {
                  value = keysym.sym;
              }

              keyInput.setKey(Key(value));
              keyInput.setType(event.type == SDL_KEYDOWN ? KeyInput::Pressed
                                                         : KeyInput::Released);
              keyInput.setShiftPressed((keysym.mod & KMOD_SHIFT) != 0);
              keyInput.setControlPressed((keysym.mod & KMOD_CTRL) != 0);
              keyInput.setAltPressed((keysym.mod & KMOD_ALT) != 0);
              keyInput.setMetaPressed((keysym.mod & KMOD_GUI) != 0);
              keyInput.setNumericPad(isNumericPadKey(keysym.scancode));

              mKeyInputQueue.push(keyInput);
              break;
          }

          case SDL_TEXTINPUT:
              mTextInputQueue.push(TextInput(event.text.text));
              break;

          case SDL_MOUSEBUTTONDOWN:
              mMouseDown = true;
              mMouseX = event.button.x;
              mMouseY = event.button.y;
              mouseInput.setX(event.button.x);
              mouseInput.setY(event.button.y);
              mouseInput.setButton(convertMouseButton(event.button.button));
              mouseInput.setType(MouseInput::Pressed);
              mouseInput.setTimeStamp(event.button.timestamp);
              mMouseInputQueue.push(mouseInput);
              break;

          case SDL_MOUSEBUTTONUP:
              mMouseDown = false;
              mMouseX = event.button.x;
              mMouseY = event.button.y;
              mouseInput.setX(event.button.x);
              mouseInput.setY(event.button.y);
              mouseInput.setButton(convertMouseButton(event.button.button));
              mouseInput.setType(MouseInput::Released);
              mouseInput.setTimeStamp(event.button.timestamp);
              mMouseInputQueue.push(mouseInput);
              break;

          case SDL_MOUSEMOTION:
              mMouseX = event.motion.x;
              mMouseY = event.motion.y;
              mouseInput.setX(event.motion.x);
              mouseInput.setY(event.motion.y);
              mouseInput.setButton(MouseInput::Empty);
              mouseInput.setType(MouseInput::Moved);
              mouseInput.setTimeStamp(event.motion.timestamp);
              mMouseInputQueue.push(mouseInput);
              break;

          case SDL_MOUSEWHEEL:
          {
              /*
               * The wheel event has no position before SDL 2.26, so the
               * position of the last mouse event is used. The direction of
               * the y value already follows the user's system preference,
               * so no correction for SDL_MOUSEWHEEL_FLIPPED is made. Scroll
               * amounts below one step, as reported by some touchpads, are
               * ignored.
               */
              int x = mMouseX;
              int y = mMouseY;
#if SDL_VERSION_ATLEAST(2, 26, 0)
              x = event.wheel.mouseX;
              y = event.wheel.mouseY;
#endif
              int steps = event.wheel.y;

              mouseInput.setX(x);
              mouseInput.setY(y);
              mouseInput.setButton(MouseInput::Empty);
              mouseInput.setType(steps > 0 ? MouseInput::WheelMovedUp
                                           : MouseInput::WheelMovedDown);
              mouseInput.setTimeStamp(event.wheel.timestamp);

              for (steps = steps < 0 ? -steps : steps; steps > 0; --steps)
              {
                  mMouseInputQueue.push(mouseInput);
              }
              break;
          }

          case SDL_WINDOWEVENT:
              /*
               * This occurs when the mouse leaves the window and the Gui-chan
               * application loses its mousefocus.
               */
              if (event.window.event == SDL_WINDOWEVENT_LEAVE)
              {
                  mMouseInWindow = false;

                  if (!mMouseDown)
                  {
                      mouseInput.setX(-1);
                      mouseInput.setY(-1);
                      mouseInput.setButton(MouseInput::Empty);
                      mouseInput.setType(MouseInput::Moved);
                      mouseInput.setTimeStamp(event.window.timestamp);
                      mMouseInputQueue.push(mouseInput);
                  }
              }
              else if (event.window.event == SDL_WINDOWEVENT_ENTER)
              {
                  mMouseInWindow = true;
              }
              break;

        } // end switch
    }

    int SDL2Input::convertMouseButton(int button)
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

    bool SDL2Input::isNumericPadKey(SDL_Scancode scancode)
    {
        return (scancode >= SDL_SCANCODE_KP_DIVIDE
                && scancode <= SDL_SCANCODE_KP_PERIOD)
            || scancode == SDL_SCANCODE_KP_EQUALS
            || scancode == SDL_SCANCODE_KP_COMMA
            || scancode == SDL_SCANCODE_KP_EQUALSAS400
            || (scancode >= SDL_SCANCODE_KP_00
                && scancode <= SDL_SCANCODE_KP_HEXADECIMAL);
    }

    int SDL2Input::convertSDLEventToGuichanKeyValue(SDL_Event event)
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
              value = Key::Space;
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
          case SDLK_PRINTSCREEN:
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
          case SDLK_NUMLOCKCLEAR:
              value = Key::NumLock;
              break;
          case SDLK_CAPSLOCK:
              value = Key::CapsLock;
              break;
          case SDLK_SCROLLLOCK:
              value = Key::ScrollLock;
              break;
          case SDLK_LGUI:
              value = Key::LeftMeta;
              break;
          case SDLK_RGUI:
              value = Key::RightMeta;
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
          case SDLK_KP_DIVIDE:
              value = '/';
              break;
          case SDLK_KP_MULTIPLY:
              value = '*';
              break;
          case SDLK_KP_MINUS:
              value = '-';
              break;
          case SDLK_KP_PLUS:
              value = '+';
              break;
          case SDLK_KP_EQUALS:
              value = '=';
              break;

          default:
              break;
        }

        // With num lock on the numeric pad produces digits, with num lock
        // off it acts as navigation keys.
        if (event.key.keysym.mod & KMOD_NUM)
        {
            switch (event.key.keysym.sym)
            {
              case SDLK_KP_0:
                  value = '0';
                  break;
              case SDLK_KP_1:
                  value = '1';
                  break;
              case SDLK_KP_2:
                  value = '2';
                  break;
              case SDLK_KP_3:
                  value = '3';
                  break;
              case SDLK_KP_4:
                  value = '4';
                  break;
              case SDLK_KP_5:
                  value = '5';
                  break;
              case SDLK_KP_6:
                  value = '6';
                  break;
              case SDLK_KP_7:
                  value = '7';
                  break;
              case SDLK_KP_8:
                  value = '8';
                  break;
              case SDLK_KP_9:
                  value = '9';
                  break;
              case SDLK_KP_PERIOD:
                  value = '.';
                  break;
              default:
                  break;
            }
        }
        else
        {
            switch (event.key.keysym.sym)
            {
              case SDLK_KP_0:
                  value = Key::Insert;
                  break;
              case SDLK_KP_1:
                  value = Key::End;
                  break;
              case SDLK_KP_2:
                  value = Key::Down;
                  break;
              case SDLK_KP_3:
                  value = Key::PageDown;
                  break;
              case SDLK_KP_4:
                  value = Key::Left;
                  break;
              case SDLK_KP_6:
                  value = Key::Right;
                  break;
              case SDLK_KP_7:
                  value = Key::Home;
                  break;
              case SDLK_KP_8:
                  value = Key::Up;
                  break;
              case SDLK_KP_9:
                  value = Key::PageUp;
                  break;
              case SDLK_KP_PERIOD:
                  value = Key::Delete;
                  break;
              default:
                  break;
            }
        }

        return value;
    }
}
