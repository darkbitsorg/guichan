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

#include "guichan/sdl2/sdl2graphics.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/image.hpp"
#include "guichan/sdl2/sdl2image.hpp"

namespace gcn
{
    SDL2Graphics::SDL2Graphics(SDL_Renderer* renderer)
    {
        mRenderer = NULL;
        setTarget(renderer);
    }

    void SDL2Graphics::_beginDraw()
    {
        if (mRenderer == NULL)
        {
            throw GCN_EXCEPTION("No renderer set, call setTarget first.");
        }

        Rectangle area;
        area.x = 0;
        area.y = 0;

        // The logical size is what drawing coordinates refer to when it
        // has been set, otherwise coordinates are in output pixels.
        SDL_RenderGetLogicalSize(mRenderer, &area.width, &area.height);

        if (area.width == 0 || area.height == 0)
        {
            SDL_GetRendererOutputSize(mRenderer, &area.width, &area.height);
        }

        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);

        pushClipArea(area);
    }

    void SDL2Graphics::_endDraw()
    {
        popClipArea();
    }

    void SDL2Graphics::setTarget(SDL_Renderer* renderer)
    {
        mRenderer = renderer;

        if (mRenderer != NULL)
        {
            SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        }
    }

    SDL_Renderer* SDL2Graphics::getTarget() const
    {
        return mRenderer;
    }

    bool SDL2Graphics::pushClipArea(Rectangle area)
    {
        bool result = Graphics::pushClipArea(area);
        updateClipRect();
        return result;
    }

    void SDL2Graphics::popClipArea()
    {
        Graphics::popClipArea();
        updateClipRect();
    }

    void SDL2Graphics::updateClipRect()
    {
        if (mClipStack.empty())
        {
            SDL_RenderSetClipRect(mRenderer, NULL);
            return;
        }

        const ClipRectangle& carea = mClipStack.top();
        SDL_Rect rect;
        rect.x = carea.x;
        rect.y = carea.y;
        rect.w = carea.width;
        rect.h = carea.height;

        SDL_RenderSetClipRect(mRenderer, &rect);
    }

    void SDL2Graphics::applyColor()
    {
        SDL_SetRenderDrawColor(mRenderer,
                               mColor.r,
                               mColor.g,
                               mColor.b,
                               mColor.a);
    }

    void SDL2Graphics::drawImage(const Image* image,
                                 int srcX,
                                 int srcY,
                                 int dstX,
                                 int dstY,
                                 int width,
                                 int height)
    {
        const SDL2Image* srcImage = dynamic_cast<const SDL2Image*>(image);

        if (srcImage == NULL)
        {
            throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an SDL2Image.");
        }

        if (srcImage->getTexture() == NULL)
        {
            throw GCN_EXCEPTION("Trying to draw an image that has not been converted to display format.");
        }

        SDL_Rect src;
        SDL_Rect dst;
        src.x = srcX;
        src.y = srcY;
        src.w = width;
        src.h = height;
        dst.x = dstX;
        dst.y = dstY;
        dst.w = width;
        dst.h = height;

        drawSDLTexture(srcImage->getTexture(), src, dst);
    }

    void SDL2Graphics::drawSDLTexture(SDL_Texture* texture,
                                      SDL_Rect source,
                                      SDL_Rect destination)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        destination.x += top.xOffset;
        destination.y += top.yOffset;

        SDL_RenderCopy(mRenderer, texture, &source, &destination);
    }

    void SDL2Graphics::fillRectangle(const Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        SDL_Rect rect;
        rect.x = rectangle.x + top.xOffset;
        rect.y = rectangle.y + top.yOffset;
        rect.w = rectangle.width;
        rect.h = rectangle.height;

        applyColor();
        SDL_RenderFillRect(mRenderer, &rect);
    }

    void SDL2Graphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        applyColor();
        SDL_RenderDrawPoint(mRenderer, x + top.xOffset, y + top.yOffset);
    }

    void SDL2Graphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        applyColor();
        SDL_RenderDrawLine(mRenderer,
                           x1 + top.xOffset,
                           y1 + top.yOffset,
                           x2 + top.xOffset,
                           y2 + top.yOffset);
    }

    void SDL2Graphics::drawRectangle(const Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        SDL_Rect rect;
        rect.x = rectangle.x + top.xOffset;
        rect.y = rectangle.y + top.yOffset;
        rect.w = rectangle.width;
        rect.h = rectangle.height;

        applyColor();
        SDL_RenderDrawRect(mRenderer, &rect);
    }

    void SDL2Graphics::setColor(const Color& color)
    {
        mColor = color;
    }

    const Color& SDL2Graphics::getColor() const
    {
        return mColor;
    }
}
