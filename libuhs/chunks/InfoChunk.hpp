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

#ifndef LIBUHS_INFOCHUNK_HPP
#define LIBUHS_INFOCHUNK_HPP

#include "BasicChunk.hpp"
#include <cstdint>
#include <string>

namespace libuhs
{
  struct InfoChunk: public BasicChunk
  {
    /** \brief default constructor */
    InfoChunk();

    /** \brief destructor */
    ~InfoChunk() {}


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


    /** \brief Resets all data members to zero / empty.
     */
    void reset();


    uint32_t fileLength; /**< length of the file */
    std::string date; /**< date of creation? */
    std::string time; /**< time of creation? */
    std::string author; /**< author's name */
    std::string publisher; /**< publisher of the game */
    std::string copyrightNotice; /**< copyright notice */
    std::string authorNote; /**< Note about the author */
    std::string gameNote; /**< notes about the game */
  }; // struct
} // namespace

#endif // LIBUHS_INFOCHUNK_HPP
