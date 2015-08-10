/*
 -------------------------------------------------------------------------------
    This file is part of libuhs.
    Copyright (C) 2015  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef LIBUHS_CHUNKREADER_HPP
#define LIBUHS_CHUNKREADER_HPP

#include <memory>
#include <vector>
#include "chunks/BasicChunk.hpp"

namespace libuhs
{
  /// "class" for reading a series of chunks
  struct ChunkReader
  {
    /** \brief tries to read a series of chunks from an input stream
     *
     * \param input    the input stream
     * \param maxLine  maximum number of lines that shall be read
     * \param lineOffset   the line number offset of the next (i.e. first) line
     * \param key          decryption key
     * \param chunks      a vector of chunks that will be filled with all chunks that were read
     * \return Returns true, if the read operation was successful.
     *         Returns false otherwise.
     */
    static bool read(std::istream& input, const uint32_t maxLines, const uint32_t lineOffset, const std::string& key, std::vector<std::unique_ptr<BasicChunk> >& chunks);
  }; //class
} //namespace

#endif // LIBUHS_CHUNKREADER_HPP
