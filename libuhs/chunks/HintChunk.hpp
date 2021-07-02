/*
 -------------------------------------------------------------------------------
    This file is part of libuhs.
    Copyright (C) 2015, 2021  Dirk Stolle

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

#ifndef LIBUHS_HINTCHUNK_HPP
#define LIBUHS_HINTCHUNK_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "BasicChunk.hpp"

namespace libuhs
{
  struct HintChunk: public BasicChunk
  {
    //constructor
    HintChunk(const uint32_t start, const std::string& lbl, const std::vector<std::string>& theHints);

    //destructor
    ~HintChunk() {}


    /** \brief Gets the type of the chunk.
     *
     * \return Returns the type of the chunk.
     */
    ChunkType getType() const override;


    /** \brief Tries to read the rest of the chunk from the given stream.
     *
     * \param input   input stream
     * \param linesTotal  number of total lines in this chunk
     * \return Returns true, if chunk was read successfully.
     *         Returns false, if read operation failed.
     */
    bool readFromStream(std::istream& input, const unsigned int linesTotal) override;

    uint32_t startingLine;
    std::vector<std::string> hints;


    /** \brief equality operator
     *
     * \param other   the other hint chunk
     * \return Returns true, if the other hint chunk is equal to this one.
     */
    bool operator==(const HintChunk& other) const;
  }; // struct
} // namespace

#endif // LIBUHS_HINTCHUNK_HPP
