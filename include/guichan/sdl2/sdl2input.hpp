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

#ifndef GCN_SDL2INPUT_HPP
#define GCN_SDL2INPUT_HPP

#include <queue>

#include "SDL.h"

#include "guichan/input.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/platform.hpp"
#include "guichan/textinput.hpp"

namespace gcn
{
    class Key;

    /**
     * SDL2 implementation of Input.
     *
     * Every SDL_KEYDOWN and SDL_KEYUP event becomes a KeyInput. Keys that
     * do not produce text are mapped to the Key constants, other keys
     * carry their SDL key code, which is the character of the key without
     * modifiers, so Ctrl+V is delivered as the key 'v' with
     * isControlPressed() set.
     *
     * Entered text is taken from SDL_TEXTINPUT events and delivered
     * through the text queue as UTF-8 strings, so hasTextInput() returns
     * true and the Gui never derives text from key presses. This makes
     * text entry work for any layout, dead keys, compose sequences and
     * input methods. The application has to enable text input with
     * SDL_StartTextInput() for these events to arrive, and can use
     * SDL_SetTextInputRect() to let input methods place their candidate
     * window next to the focused widget. Text being composed by an input
     * method (SDL_TEXTEDITING) is not delivered, only the committed text.
     */
    class GCN_EXTENSION_DECLSPEC SDL2Input : public Input
    {
    public:

        /**
         * Constructor.
         */
        SDL2Input();

        /**
         * Pushes an SDL event. It should be called at least once per frame to
         * update input with user input.
         *
         * @param event an event from SDL.
         */
        virtual void pushInput(SDL_Event event);

        /**
         * Polls all input. It exists for input driver compatibility. If you
         * only use SDL and plan sticking with SDL you can safely ignore this
         * function as it in the SDL case does nothing.
         */
        virtual void _pollInput() { }


        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual MouseInput dequeueMouseInput();

        virtual bool hasTextInput();

        virtual bool isTextQueueEmpty();

        virtual TextInput dequeueTextInput();

    protected:
        /**
         * Converts a mouse button from SDL to a Guichan mouse button
         * representation.
         *
         * @param button an SDL mouse button.
         * @return a Guichan mouse button.
         */
        int convertMouseButton(int button);

        /**
         * Converts an SDL key event to a Guichan key value.
         *
         * @param event The SDL key event to convert.
         * @return A Guichan key value. -1 if no conversion took place.
         * @see Key
         */
        int convertSDLEventToGuichanKeyValue(SDL_Event event);

        /**
         * Checks whether an SDL scan code belongs to the numeric pad.
         *
         * @param scancode an SDL scan code.
         * @return true if the scan code belongs to the numeric pad.
         */
        bool isNumericPadKey(SDL_Scancode scancode);

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;
        std::queue<TextInput> mTextInputQueue;

        bool mMouseDown;
        bool mMouseInWindow;
        int mMouseX;
        int mMouseY;
    };
}

#endif // end GCN_SDL2INPUT_HPP
