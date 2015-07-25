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

#ifndef TEXTCHUNK_HPP
#define TEXTCHUNK_HPP

#include <cstdint>
#include <string>
#include "BasicChunk.hpp"

namespace libuhs
{
  struct TextChunk: public BasicChunk
  {
    //constructor
    TextChunk(const uint32_t start, const std::string& lbl, const uint32_t _offset, const uint32_t len);

    //destructor
    ~TextChunk() {}


    /** \brief gets the type of the hunk
     *
     * \return returns the type of the hunk as enumeration
     */
    virtual ChunkType getType() const;

    uint32_t startingLine;
    std::string label;
    uint32_t offset;
    uint32_t length;
  }; //struct
} //namespace

#endif // TEXTCHUNK_HPP
