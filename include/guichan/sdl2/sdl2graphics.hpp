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

#ifndef GCN_SDL2GRAPHICS_HPP
#define GCN_SDL2GRAPHICS_HPP

#include "SDL.h"

#include "guichan/color.hpp"
#include "guichan/graphics.hpp"
#include "guichan/platform.hpp"

namespace gcn
{
    class Image;
    class Rectangle;

    /**
     * SDL2 implementation of the Graphics. It draws with an SDL_Renderer,
     * so images need to be SDL2Image objects converted to display format,
     * which turns them into textures of that renderer. The application
     * is responsible for clearing and presenting the renderer, for example
     * with SDL_RenderClear before Gui::draw and SDL_RenderPresent after
     * it.
     */
    class GCN_EXTENSION_DECLSPEC SDL2Graphics : public Graphics
    {
    public:

        // Needed so that drawImage(gcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         *
         * @param renderer the renderer to draw with. May be NULL, in which
         *                 case a renderer has to be set with setTarget
         *                 before drawing.
         */
        SDL2Graphics(SDL_Renderer* renderer = NULL);

        /**
         * Sets the target SDL_Renderer to draw with. The renderer's draw
         * blend mode is set to SDL_BLENDMODE_BLEND so that the alpha of
         * the current color is respected.
         *
         * @param renderer the renderer to draw with.
         */
        virtual void setTarget(SDL_Renderer* renderer);

        /**
         * Gets the target SDL_Renderer.
         *
         * @return the target SDL_Renderer.
         */
        virtual SDL_Renderer* getTarget() const;

        /**
         * Draws an SDL_Texture with the target renderer. Normaly you'll
         * use drawImage, but if you want to write SDL specific code
         * this function might come in handy.
         *
         * NOTE: The clip areas will be taken into account.
         */
        virtual void drawSDLTexture(SDL_Texture* texture,
                                    SDL_Rect source,
                                    SDL_Rect destination);


        // Inherited from Graphics

        virtual void _beginDraw();

        virtual void _endDraw();

        virtual bool pushClipArea(Rectangle area);

        virtual void popClipArea();

        virtual void drawImage(const Image* image,
                               int srcX,
                               int srcY,
                               int dstX,
                               int dstY,
                               int width,
                               int height);

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawRectangle(const Rectangle& rectangle);

        virtual void fillRectangle(const Rectangle& rectangle);

        virtual void setColor(const Color& color);

        virtual const Color& getColor() const;

    protected:
        /**
         * Applies the top of the clip stack as the renderer's clip
         * rectangle, or removes the clip rectangle when the stack is
         * empty.
         */
        void updateClipRect();

        /**
         * Sets the current color as the renderer's draw color.
         */
        void applyColor();

        SDL_Renderer* mRenderer;
        Color mColor;
    };
}

#endif // end GCN_SDL2GRAPHICS_HPP
