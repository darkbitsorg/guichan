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

#ifndef GCN_HGEIMAGELOADER_HPP
#define GCN_HGEIMAGELOADER_HPP

#include <hge.h>
#include <hgeresource.h>

#include "guichan/platform.hpp"
#include "guichan/imageloader.hpp"

namespace gcn
{
    class Image;

    /**
     * HGE implementation of ImageLoader.
     * 
     * @author Kevin Lynx
     * @since 0.6.1
     */
    class GCN_EXTENSION_DECLSPEC HGEImageLoader : public ImageLoader
    {
    public:

        /**
         * Constructor.
         */
        HGEImageLoader();

        /**
         * Destructor.
         */
        ~HGEImageLoader();

        /**
         * Loads an image with the HGE resource manager.
         *
         * NOTE: The functions Image::getPixel and Image::putPixel 
         *       are only guaranteed to work before an image has 
         *       been converted to display format.
         *
         * @param manager The manager to use.
         * @param name The name of the image.
         * @param resourceGroup The resource group.
         * @param convertToDisplayFormat True if the image should be converted
         *                               to display, false otherwise.
         */
        virtual Image* load(hgeResourceManager* manager, char* name, int resourceGroup, bool convertToDisplayFormat);


        // Inherited from ImageLoader

        virtual Image* load(const std::string &filename, bool convertToDisplayFormat = true);

    protected:
        static HGE *mHGE;
    };
}

#endif // end GCN_HGEIMAGELOADER_HPP
