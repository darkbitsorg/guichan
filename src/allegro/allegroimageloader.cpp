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

#include "guichan/allegro/allegroimageloader.hpp"

#include "guichan/allegro/allegroimage.hpp"
#include "guichan/exception.hpp"

namespace gcn
{
    Image* AllegroImageLoader::load(const std::string& filename, bool convertToDisplayFormat)
    {
#if !(ALLEGRO_VERSION == 4 && ALLEGRO_SUB_VERSION == 0)
        int colconv = get_color_conversion();
#endif

        set_color_conversion(COLORCONV_NONE);

        PALETTE pal;
        BITMAP *bmp = loadBitmap(filename, pal);

        if (bmp == NULL)
        {
            throw GCN_EXCEPTION(std::string("Unable to load: ") + filename);
        }

        BITMAP *bmp2 = create_bitmap_ex(32, bmp->w, bmp->h);

        if (bmp2 == NULL)
        {
            throw GCN_EXCEPTION(std::string("Not enough memory to load: ") + filename);
        }

        set_palette(pal);
        blit(bmp, bmp2, 0, 0, 0, 0, bmp->w, bmp->h);
        destroy_bitmap(bmp);

#if (ALLEGRO_VERSION == 4 && ALLEGRO_SUB_VERSION == 0)
        set_color_conversion(COLORCONV_TOTAL);
#else
        set_color_conversion(colconv);
#endif

        Image *image = new AllegroImage(bmp2, true);

        if (convertToDisplayFormat)
        {
            image->convertToDisplayFormat();
        }

        return image;
    }

    BITMAP* AllegroImageLoader::loadBitmap(const std::string& filename, PALETTE pal)
    {
        return load_bitmap(filename.c_str(), pal);
    }
}
