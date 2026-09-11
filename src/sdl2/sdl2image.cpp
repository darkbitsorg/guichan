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

#include "guichan/exception.hpp"

namespace gcn
{
    namespace
    {
        /**
         * Reads a raw pixel value from a pixel buffer.
         */
        Uint32 readPixel(const Uint8* p, int bpp)
        {
            switch (bpp)
            {
              case 1:
                  return *p;

              case 2:
                  return *(const Uint16*)p;

              case 3:
                  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                  {
                      return p[0] << 16 | p[1] << 8 | p[2];
                  }
                  else
                  {
                      return p[0] | p[1] << 8 | p[2] << 16;
                  }

              case 4:
                  return *(const Uint32*)p;

              default:
                  return 0;
            }
        }

        /**
         * Writes a raw pixel value to a pixel buffer.
         */
        void writePixel(Uint8* p, int bpp, Uint32 pixel)
        {
            switch (bpp)
            {
              case 1:
                  *p = pixel;
                  break;

              case 2:
                  *(Uint16*)p = pixel;
                  break;

              case 3:
                  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                  {
                      p[0] = (pixel >> 16) & 0xff;
                      p[1] = (pixel >> 8) & 0xff;
                      p[2] = pixel & 0xff;
                  }
                  else
                  {
                      p[0] = pixel & 0xff;
                      p[1] = (pixel >> 8) & 0xff;
                      p[2] = (pixel >> 16) & 0xff;
                  }
                  break;

              case 4:
                  *(Uint32*)p = pixel;
                  break;
            }
        }

        /**
         * Gets a pointer to a pixel of a surface, which must be locked.
         */
        Uint8* pixelAddress(SDL_Surface* surface, int x, int y)
        {
            return (Uint8*)surface->pixels
                + y * surface->pitch
                + x * surface->format->BytesPerPixel;
        }
    }

    SDL2Image::SDL2Image(SDL_Surface* surface, SDL_Renderer* renderer, bool autoFree)
    {
        mSurface = surface;
        mTexture = NULL;
        mRenderer = renderer;
        mWidth = surface != NULL ? surface->w : 0;
        mHeight = surface != NULL ? surface->h : 0;
        mAutoFree = autoFree;
    }

    SDL2Image::SDL2Image(SDL_Texture* texture, int width, int height, bool autoFree)
    {
        mSurface = NULL;
        mTexture = texture;
        mRenderer = NULL;
        mWidth = width;
        mHeight = height;
        mAutoFree = autoFree;
    }

    SDL2Image::~SDL2Image()
    {
        if (mAutoFree)
        {
            free();
        }
    }

    SDL_Surface* SDL2Image::getSurface() const
    {
        return mSurface;
    }

    SDL_Texture* SDL2Image::getTexture() const
    {
        return mTexture;
    }

    int SDL2Image::getWidth() const
    {
        if (mSurface == NULL && mTexture == NULL)
        {
            throw GCN_EXCEPTION("Trying to get the width of a non loaded image.");
        }

        return mWidth;
    }

    int SDL2Image::getHeight() const
    {
        if (mSurface == NULL && mTexture == NULL)
        {
            throw GCN_EXCEPTION("Trying to get the height of a non loaded image.");
        }

        return mHeight;
    }

    Color SDL2Image::getPixel(int x, int y)
    {
        if (mTexture != NULL)
        {
            throw GCN_EXCEPTION("Image has been converted to display format");
        }

        if (mSurface == NULL)
        {
            throw GCN_EXCEPTION("Trying to get a pixel from a non loaded image.");
        }

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
        {
            throw GCN_EXCEPTION("Coordinates outside of the image");
        }

        SDL_LockSurface(mSurface);

        Uint32 pixel = readPixel(pixelAddress(mSurface, x, y),
                                 mSurface->format->BytesPerPixel);

        Uint8 r, g, b, a;
        SDL_GetRGBA(pixel, mSurface->format, &r, &g, &b, &a);

        SDL_UnlockSurface(mSurface);

        return Color(r, g, b, a);
    }

    void SDL2Image::putPixel(int x, int y, const Color& color)
    {
        if (mTexture != NULL)
        {
            throw GCN_EXCEPTION("Image has been converted to display format");
        }

        if (mSurface == NULL)
        {
            throw GCN_EXCEPTION("Trying to put a pixel in a non loaded image.");
        }

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
        {
            throw GCN_EXCEPTION("Coordinates outside of the image");
        }

        Uint32 pixel = SDL_MapRGBA(mSurface->format,
                                   color.r, color.g, color.b, color.a);

        SDL_LockSurface(mSurface);

        writePixel(pixelAddress(mSurface, x, y),
                   mSurface->format->BytesPerPixel,
                   pixel);

        SDL_UnlockSurface(mSurface);
    }

    void SDL2Image::convertToDisplayFormat()
    {
        if (mTexture != NULL)
        {
            throw GCN_EXCEPTION("Image has already been converted to display format");
        }

        if (mSurface == NULL)
        {
            throw GCN_EXCEPTION("Trying to convert a non loaded image to display format.");
        }

        if (mRenderer == NULL)
        {
            throw GCN_EXCEPTION("Trying to convert an image to display format without a renderer.");
        }

        // Pink is the color key convention of the Guichan image loaders.
        // SDL_CreateTextureFromSurface turns color keyed pixels into
        // transparent ones.
        const Uint32 pink = SDL_MapRGB(mSurface->format, 255, 0, 255);
        const int bpp = mSurface->format->BytesPerPixel;
        bool hasPink = false;

        SDL_LockSurface(mSurface);

        for (int y = 0; y < mHeight && !hasPink; ++y)
        {
            for (int x = 0; x < mWidth; ++x)
            {
                if (readPixel(pixelAddress(mSurface, x, y), bpp) == pink)
                {
                    hasPink = true;
                    break;
                }
            }
        }

        SDL_UnlockSurface(mSurface);

        if (hasPink)
        {
            SDL_SetColorKey(mSurface, SDL_TRUE, pink);
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, mSurface);

        if (texture == NULL)
        {
            throw GCN_EXCEPTION(std::string("Unable to convert image to display format: ")
                                + SDL_GetError());
        }

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        if (mAutoFree)
        {
            SDL_FreeSurface(mSurface);
        }

        mSurface = NULL;
        mTexture = texture;
    }

    void SDL2Image::free()
    {
        if (mSurface != NULL)
        {
            SDL_FreeSurface(mSurface);
            mSurface = NULL;
        }

        if (mTexture != NULL)
        {
            SDL_DestroyTexture(mTexture);
            mTexture = NULL;
        }
    }
}
