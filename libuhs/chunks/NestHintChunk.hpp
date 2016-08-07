/*
 -------------------------------------------------------------------------------
    This file is part of libuhs.
    Copyright (C) 2015  Dirk Stolle

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

#ifndef LIBUHS_NESTHINTCHUNK_HPP
#define LIBUHS_NESTHINTCHUNK_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "BasicChunk.hpp"
/* We might have to #include both <boost/variant.hpp> and LinkChunk.cpp to
   completely parse NestHint Chunks. */

namespace libuhs
{
  struct NestHintChunk: public BasicChunk
  {
    ///constructor
    NestHintChunk(const uint32_t start, const std::string& lbl, const std::string& key, const std::vector<std::string>& theHints);

    ///destructor
    ~NestHintChunk() {}


    /** \brief gets the type of the hunk
     *
     * \return returns the type of the hunk as enumeration
     */
    virtual ChunkType getType() const;


    /** \brief tries to read the rest of the chunk from the given stream
     *
     * \param input   input stream
     * \param linesTotal  number of total lines in this hunk
     * \return Returns true, if chunk was read successfully.
     *         Returns false, if read operation failed.
     */
    bool readFromStream(std::istream& input, const unsigned int linesTotal) override;

    uint32_t startingLine;
    std::string decryptionKey; /**< the key that will be used for decryption */
    std::vector<std::string> hints;
  }; //struct
} //namespace

#endif // LIBUHS_NESTHINTCHUNK_HPP
