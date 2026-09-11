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

#include "guichan/sdl2/sdl2image.hpp"

#include "SDL_image.h"

#include "guichan/exception.hpp"
#include "guichan/sdl2/sdl2imageloader.hpp"

namespace gcn
{
    SDL2ImageLoader::SDL2ImageLoader(SDL_Renderer* renderer)
    {
        mRenderer = renderer;
    }

    void SDL2ImageLoader::setRenderer(SDL_Renderer* renderer)
    {
        mRenderer = renderer;
    }

    SDL_Renderer* SDL2ImageLoader::getRenderer() const
    {
        return mRenderer;
    }

    Image* SDL2ImageLoader::load(const std::string& filename,
                                 bool convertToDisplayFormat)
    {
        SDL_Surface *loadedSurface = loadSDLSurface(filename);

        if (loadedSurface == NULL)
        {
            throw GCN_EXCEPTION(
                    std::string("Unable to load image file: ") + filename);
        }

        SDL_Surface *surface = convertToStandardFormat(loadedSurface);
        SDL_FreeSurface(loadedSurface);

        if (surface == NULL)
        {
            throw GCN_EXCEPTION(
                    std::string("Not enough memory to load: ") + filename);
        }

        Image *image = new SDL2Image(surface, mRenderer, true);

        if (convertToDisplayFormat)
        {
            image->convertToDisplayFormat();
        }

        return image;
    }

    SDL_Surface* SDL2ImageLoader::loadSDLSurface(const std::string& filename)
    {
        return IMG_Load(filename.c_str());
    }

    SDL_Surface* SDL2ImageLoader::convertToStandardFormat(SDL_Surface* surface)
    {
        return SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    }
}
