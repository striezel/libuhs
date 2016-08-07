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

#ifndef LIBUHS_LINKCHUNK_HPP
#define LIBUHS_LINKCHUNK_HPP

#include <cstdint>
#include <string>
#include "BasicChunk.hpp"

namespace libuhs
{
  struct LinkChunk: public BasicChunk
  {
    ///default constructor
    LinkChunk(const uint32_t start = 0, const std::string& lbl=std::string(), const uint32_t dest=0);


    ///destructor
    ~LinkChunk() {}


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
    virtual bool readFromStream(std::istream& input, const unsigned int linesTotal) override;


    /** \brief equality operator
     *
     * \param other   the other link chunk
     * \return Returns true, if the other link chunk is equal to this one.
     */
    bool operator==(const LinkChunk& other) const;


    uint32_t startingLine;
    uint32_t destinationLine; /**< destination line of the link */
  }; //struct
} //namespace

#endif // LIBUHS_LINKCHUNK_HPP
