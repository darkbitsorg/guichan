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

#ifndef GCN_SDL2IMAGELOADER_HPP
#define GCN_SDL2IMAGELOADER_HPP

#include "guichan/imageloader.hpp"
#include "guichan/platform.hpp"

#include "SDL.h"

namespace gcn
{
    class Image;

    /**
     * SDL2 implementation of ImageLoader. Images are loaded with SDL2_image
     * into a 32 bit RGBA surface and become textures of the renderer given
     * to the loader when they are converted to display format.
     *
     * NOTE: Converting an image to display format makes its pixels
     *       inaccessible. ImageFont reads the pixels of its image to find
     *       the glyphs, so it loads with convertToDisplayFormat set to
     *       false and converts the image afterwards. Do the same for any
     *       image whose pixels you need to inspect or change.
     */
    class GCN_EXTENSION_DECLSPEC SDL2ImageLoader : public ImageLoader
    {
    public:

        /**
         * Constructor.
         *
         * @param renderer the renderer the loaded images create their
         *                 textures for. May be NULL, in which case a
         *                 renderer has to be set with setRenderer before
         *                 loading images.
         */
        SDL2ImageLoader(SDL_Renderer* renderer = NULL);

        /**
         * Sets the renderer the loaded images create their textures for.
         * Images loaded before the call are not affected.
         *
         * @param renderer the renderer to create textures for.
         * @see getRenderer
         */
        void setRenderer(SDL_Renderer* renderer);

        /**
         * Gets the renderer the loaded images create their textures for.
         *
         * @return the renderer to create textures for.
         * @see setRenderer
         */
        SDL_Renderer* getRenderer() const;


        // Inherited from ImageLoader

        virtual Image* load(const std::string& filename, bool convertToDisplayFormat = true);

    protected:
        virtual SDL_Surface* loadSDLSurface(const std::string& filename);
        virtual SDL_Surface* convertToStandardFormat(SDL_Surface* surface);

        SDL_Renderer* mRenderer;
    };
}

#endif // end GCN_SDL2IMAGELOADER_HPP
