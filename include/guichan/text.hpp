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

#ifndef GCN_TEXT_HPP
#define GCN_TEXT_HPP

#include "guichan/platform.hpp"
#include "guichan/rectangle.hpp"

#include <string>
#include <vector>

namespace gcn
{
    class Font;

    /**
     * A utility class to ease working with text in widgets such as
     * TextBox and TextField. The class wraps common text operations
     * such as inserting and deleting text.
     *
     * The content is expected to be UTF-8 encoded. Caret positions and
     * columns are byte offsets into the content, so they can be used to
     * index the rows directly, but all editing and caret movement works
     * on whole UTF-8 sequences and the caret is always kept at the start
     * of a sequence.
     *
     * @since 0.9.0
     */
    class GCN_CORE_DECLSPEC Text
    {
    public:
        /**
         * Constructor.
         */
        Text();

        /**
         * Constructor.
         *
         * @param content The content of the text.
         * @since 0.9.0
         */
        Text(const std::string& content);

        /**
         * Sets the content of the text. Will completely remove 
         * any previous text and reset the caret position.
         *
         * @param content The content of the text.
         * @since 0.9.0
         */
        void setContent(const std::string& text);

        /**
         * Gets the content of the text.
         *
         * @return The content of the text.
         * @since 0.9.0
         */
        std::string getContent() const;

        /**
         * Sets the content of a row.
         *
         * @param row The row to set the text of.
         * @throws Exception when the row does not exist.
         * @since 0.9.0
         */
        void setRow(unsigned int row, const std::string& content);

        /**
         * Adds a row to the content. Calling this method will
         * not change the current caret position.
         *
         * @param row The row to add.
         * @since 0.9.0
         */
        void addRow(const std::string& row);

        /**
         * Inserts a row before the specified row position. Calling this method
         * will not change the current caret position.
         *
         * @param row The row to add.
         * @param position Inserts new row before this row.
         */
        void insertRow(const std::string& row, unsigned int position);

        /**
         * Erases the given row. Calling this method will not change the current
         * caret position.
         *
         * @param row Row to be erased.
         */
        void eraseRow(unsigned int row);

        /**
         * Gets a reference to a row.
         *
         * @param row The row to get the content of.
         * @return The reference to a row.
         * @throws Exception when no such row exists.
         * @since 0.9.0
         */
        const std::string& getRow(unsigned int row) const;

        /**
         * Inserts a UTF-8 encoded string at the current caret position.
         * Line feeds in the string split the current row. The caret is
         * moved to the end of the inserted text.
         *
         * @param text The UTF-8 encoded text to insert.
         * @since 0.9.0
         */
        void insert(const std::string& text);

        /**
         * Inserts a character at the current caret position. The character
         * is a Unicode code point which is UTF-8 encoded before insertion.
         * Values outside of the range 0 to 0x10FFFF are ignored.
         *
         * @param character The Unicode code point to insert.
         * @since 0.9.0
         */
        void insert(int character);

        /**
         * Removes a given number of characters starting at the current
         * caret position. A character is a whole UTF-8 sequence, so
         * removing one character may remove several bytes.
         *
         * If the number of characters to remove is negative
         * characters will be removed left of the caret position.
         * If the number is positive characters will be removed
         * right of the caret position. If a line feed is
         * removed the row with the line feed will be merged
         * with the row above the line feed.
         *
         * @param numberOfCharacters The number of characters to remove.
         * @since 0.9.0
         */
        void remove(int numberOfCharacters);

        /**
         * Gets the caret position.
         *
         * @return The caret position.
         * @since 0.9.0
         */
        int getCaretPosition() const;

        /**
         * Sets the caret position. The position will be
         * clamp to the dimension of the content. If the position
         * lies inside of a UTF-8 sequence the caret is moved back
         * to the start of that sequence.
         *
         * @param position The position of the caret.
         * @since 0.9.0
         */
        void setCaretPosition(int position);
        
        /**
         * Sets the caret position given an x and y coordinate in pixels
         * relative to the text. The coordinates will be clamp to the content.
         *
         * @param x The x coordinate of the caret.
         * @param y The y coordinate of the caret.
         * @param font The font to use when calculating the position.
         * @since 0.9.0
         */
        void setCaretPosition(int x, int y, Font* font);

        /**
         * Gets the column the caret is currently in.
         *
         * @return The column the caret is currently in.
         * @since 0.9.0
         */
        int getCaretColumn() const;

        /**
         * Gets the row the caret is currently in.
         *
         * @return The row the caret is currently in.
         * @since 0.9.0
         */
        int getCaretRow() const;

        /**
         * Sets the column the caret should be in. The column
         * will be clamp to the current row. If the column lies
         * inside of a UTF-8 sequence the caret is moved back to
         * the start of that sequence.
         *
         * @param column The column the caret should be in.
         * @since 0.9.0
         */
        void setCaretColumn(int column);

        /**
         * Sets the row the caret should be in. If the row lies o
         * outside of the text, the row will be set to zero or the 
         * maximum row depending on where the row lies outside of the 
         * text.
         *
         * Calling this function trigger a recalculation of the caret
         * column.
         *
         * @param row The row the caret should be in.
         * @since 0.9.0
         */
        void setCaretRow(int row);

        /**
         * Moves the caret one character to the left. At the start of
         * a row the caret moves to the end of the row above.
         *
         * @since 0.9.0
         */
        void moveCaretLeft();

        /**
         * Moves the caret one character to the right. At the end of
         * a row the caret moves to the start of the row below.
         *
         * @since 0.9.0
         */
        void moveCaretRight();

        /**
         * Gets the x coordinate of the caret in pixels given a font.
         *
         * @param font The font to use when calculating the x coordinate.
         * @return The x coorinate of the caret in pixels.
         * @since 0.9.0
         */
        int getCaretX(Font* font) const;

        /**
         * Gets the y coordinate of the caret in pixels given a font.
         *
         * @param font The font to use when calculating the y coordinate.
         * @return The y coorinate of the caret in pixels.
         * @since 0.9.0
         */
        int getCaretY(Font* font) const;

        /**
         * Gets the dimension in pixels of the text given a font. If there
         * is no text present a dimension of a white space will be returned.
         *
         * @param font The font to use when calculating the dimension.
         * @return The dimension in pixels of the text given a font.
         * @since 0.9.0
         */
        Rectangle getDimension(Font* font) const;

        /**
         * Gets the caret dimension relative to this text.
         * The returned dimension is perfect for use with Widget::showPart
         * so the caret is always shown.
         *
         * @param font The font to use when calculating the dimension.
         * @return The dimension of the caret.
         * @since 0.9.0
         */
        Rectangle getCaretDimension(Font* font) const;

        /**
         * Gets the width in pixels of a row. If the row is not
         * present in the text zero will be returned.
         *
         * @param row The row to get the width of.
         * @return The width in pixels of a row.
         * @since 0.9.0
         */ 
        int getWidth(int row, Font* font) const;

        /**
         * Gets the maximum row the caret can be in.
         * 
         * @return The maximum row the caret can be in.
         * @since 0.9.0
         */
        unsigned int getMaximumCaretRow() const;

        /**
         * Gets the maximum column of a row the caret can be in.
         * 
         * @param row The row of the caret.
         * @return The maximum column of a row the caret can be in.
         * @since 0.9.0
         */
        unsigned int getMaximumCaretRow(unsigned int row) const;

        /**
         * Gets the number of rows in the text.
         *
         * @return The number of rows in the text.
         * @since 0.9.0
         */
        unsigned int getNumberOfRows() const;

        /**
         * Gets the number of characters in the text. A character is a
         * Unicode code point, so this is not the same as the length in
         * bytes for UTF-8 content. Line feeds between rows are counted
         * as characters.
         *
         * @return The number of characters in the text.
         * @see getNumberOfColumns
         * @since 0.9.0
         */
        unsigned int getNumberOfCharacters() const;

        /**
         * Gets the number of characters in a certain row in the text.
         * A character is a Unicode code point, so this is not the same
         * as the length in bytes. Use getNumberOfColumns for the byte
         * length, which is what caret columns are measured in. If the
         * row does not exist, zero will be returned.
         *
         * @param row The row to get the number of characters in.
         * @return The number of characters in a certain row, or zero
         *         if the row does not exist.
         * @see getNumberOfColumns
         * @since 0.9.0
         */
        unsigned int getNumberOfCharacters(unsigned int row) const;

        /**
         * Gets the number of columns in a certain row in the text, that
         * is the length of the row in bytes. Caret columns are byte
         * offsets, so this is the column right after the last character
         * of the row. Use getNumberOfCharacters to count characters
         * instead. If the row does not exist, zero will be returned.
         *
         * @param row The row to get the number of columns in.
         * @return The number of columns in a certain row, or zero
         *         if the row does not exist.
         * @see getNumberOfCharacters
         * @since 0.9.0
         */
        unsigned int getNumberOfColumns(unsigned int row) const;

        /**
         * Gets the column where the UTF-8 character before a given column
         * starts. If the column is zero, zero is returned.
         *
         * @param row The content of the row.
         * @param column The column to step back from.
         * @return The column where the previous character starts.
         * @since 0.9.0
         */
        static unsigned int getPreviousCharacterColumn(const std::string& row,
                                                       unsigned int column);

        /**
         * Gets the column right after the UTF-8 character that starts at
         * a given column. If the column is at the end of the row, the
         * length of the row is returned.
         *
         * @param row The content of the row.
         * @param column The column to step forward from.
         * @return The column where the next character starts.
         * @since 0.9.0
         */
        static unsigned int getNextCharacterColumn(const std::string& row,
                                                   unsigned int column);

        /**
         * Encodes a Unicode code point as UTF-8. Values outside of the
         * range 0 to 0x10FFFF result in an empty string.
         *
         * @param character The Unicode code point to encode.
         * @return The UTF-8 encoding of the code point.
         * @since 0.9.0
         */
        static std::string encodeUtf8(int character);

    protected:

        /**
         * Calculates the caret position from the caret row and caret column.
         */
        void calculateCaretPositionFromRowAndColumn();

        /**
         * Holds the text row by row.
         */
        std::vector<std::string> mRows;

        /**
         * Holds the position of the caret. This variable should
         * always be valid.
         */
        unsigned int mCaretPosition;

        /**
         * Holds the row the caret is in. This variable should always
         * be valid.
         */
        unsigned int mCaretRow;

        /**
         * Holds the column the caret is in. This variable should always
         * be valid.
         */
        unsigned int mCaretColumn;
    };
}
#endif
