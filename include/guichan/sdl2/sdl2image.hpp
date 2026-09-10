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

#ifndef GCN_SDL2IMAGE_HPP
#define GCN_SDL2IMAGE_HPP

#include "SDL.h"

#include <string>

#include "guichan/color.hpp"
#include "guichan/platform.hpp"
#include "guichan/image.hpp"

namespace gcn
{
    /**
     * SDL2 implementation of Image. Before conversion to display format
     * the image holds an SDL_Surface, whose pixels can be read and
     * written. Conversion turns the surface into an SDL_Texture of the
     * renderer the image was created with, after which the pixels can no
     * longer be accessed.
     */
    class GCN_EXTENSION_DECLSPEC SDL2Image : public Image
    {
    public:
        /**
         * Constructor. Load an image from an SDL surface.
         *
         * NOTE: The functions getPixel and putPixel only work before an
         *       image has been converted to display format.
         *
         * @param surface the surface from which to load.
         * @param renderer the renderer the texture is created for when
         *                 the image is converted to display format.
         * @param autoFree true if the surface and the texture should
         *                 automatically be released. When false, the
         *                 surface is left to the caller after conversion
         *                 and free() needs to be called to release the
         *                 texture.
         */
        SDL2Image(SDL_Surface* surface, SDL_Renderer* renderer, bool autoFree);

        /**
         * Constructor. Load an image from an existing SDL texture. The
         * image counts as converted to display format.
         *
         * @param texture the texture from which to load.
         * @param width the width of the image.
         * @param height the height of the image.
         * @param autoFree true if the texture should automatically be
         *                 destroyed.
         */
        SDL2Image(SDL_Texture* texture, int width, int height, bool autoFree);

        /**
         * Destructor.
         */
        virtual ~SDL2Image();

        /**
         * Gets the SDL surface for the image.
         *
         * @return the SDL surface for the image, or NULL when the image
         *         has been converted to display format.
         */
        virtual SDL_Surface* getSurface() const;

        /**
         * Gets the SDL texture for the image.
         *
         * @return the SDL texture for the image, or NULL when the image
         *         has not been converted to display format.
         */
        virtual SDL_Texture* getTexture() const;


        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, const Color& color);

        /**
         * Creates a texture from the surface for the renderer given at
         * construction and releases the surface. Pixels in pink
         * (255, 0, 255) become transparent, and the texture gets the
         * blend mode SDL_BLENDMODE_BLEND so that per pixel alpha is
         * respected when drawing.
         */
        virtual void convertToDisplayFormat();

    protected:
        SDL_Surface* mSurface;
        SDL_Texture* mTexture;
        SDL_Renderer* mRenderer;
        int mWidth;
        int mHeight;
        bool mAutoFree;
    };
}

#endif // end GCN_SDL2IMAGE_HPP
