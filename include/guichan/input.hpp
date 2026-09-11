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

#ifndef GCN_INPUT_HPP
#define GCN_INPUT_HPP

#include "guichan/platform.hpp"

namespace gcn
{
    class KeyInput;
    class MouseInput;
    class TextInput;

    /**
     * Abstract class for providing functions for user input. 
     *
     * Guichan contains implementations of Input for common 
     * libraries like the Allegro library, the HGE library, 
     * and the SDL library. 
     * To make Guichan usable with other libraries, an Input 
     * class must be implemented.
     *
     * @see AllegroInput, HGEInput, OpenLayerInput, 
     *      SDLInput
     */
    class GCN_CORE_DECLSPEC Input
    {
    public:

        /**
         * Destructor.
         */
        virtual ~Input(){ }

        /**
         * Checks if the key queue is empty, or not.
         *
         * @return True if the key queue is empty, 
         *         false otherwise.
         */
        virtual bool isKeyQueueEmpty() = 0;

        /**
         * Dequeues the key input queue.
         *
         * @return The first key input in the key input queue.
         */
        virtual KeyInput dequeueKeyInput() = 0;

        /**
         * Checks if the mouse queue is empyt, or not.
         *
         * @return True if the mouse queue is empty,
         *         false otherwise.
         */
        virtual bool isMouseQueueEmpty() = 0;

        /**
         * Dequeues the mouse input queue.
         *
         * @return The first mouse input in the mouse input queue.
         */
        virtual MouseInput dequeueMouseInput() = 0;

        /**
         * Checks if the back end delivers entered text through the text
         * queue. When it does, the Gui distributes the dequeued text as
         * text events and does not derive any text from key events.
         * When it does not, the Gui synthesises a text event from each
         * pressed key that represents a character.
         *
         * The default implementation returns false, so existing back ends
         * keep working without changes.
         *
         * @return True if entered text is delivered through the text
         *         queue, false otherwise.
         * @see isTextQueueEmpty, dequeueTextInput, Gui::handleTextInput
         * @since 0.9.0
         */
        virtual bool hasTextInput();

        /**
         * Checks if the text queue is empty, or not. The default
         * implementation always reports an empty queue.
         *
         * @return True if the text queue is empty,
         *         false otherwise.
         * @see hasTextInput
         * @since 0.9.0
         */
        virtual bool isTextQueueEmpty();

        /**
         * Dequeues the text input queue. The default implementation
         * throws as its queue is always empty.
         *
         * @return The first text input in the text input queue.
         * @throws Exception when the text queue is empty.
         * @see hasTextInput
         * @since 0.9.0
         */
        virtual TextInput dequeueTextInput();

        /**
         * Polls all exsisting input. Called when input should
         * be polled. The function exists for compatibility reason
         * where some libraries need to poll input at a certain
         * logic rate.
         */
        virtual void _pollInput() = 0;
    };
}

#endif // end GCN_INPUT_HPP
