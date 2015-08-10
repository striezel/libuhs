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

#ifndef LIBUHS_HYPERPNGCHUNK_HPP
#define LIBUHS_HYPERPNGCHUNK_HPP

#include <cstdint>
#include <memory>
#include <string>
#include "BasicChunk.hpp"

namespace libuhs
{
  struct HyperPNGChunk: public BasicChunk
  {
    public:
      ///
      /** \brief default constructor
       *
       * \param label   the chunk label
       */
      HyperPNGChunk(const std::string& label = "");


      ///destructor
      ~HyperPNGChunk() {}


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


      /** \brief writes the PNG file data to the given file
       *
       * \param fileName   name of the destination file
       * \return Returns true, if data was written to the file.
       *         Returns false, if an error occurred.
       */
      bool writePNGFile(const std::string& fileName) const;


      /** \brief gets the size of the PNG file in bytes
       *
       * \return Returns the PNG's file size in bytes.
       *         Returns zero, if no PNG file has be read yet.
       */
      uint32_t getPNGSize() const;


      static const uint32_t MaximumFileSize; /**< constant value that limits the maximum file size */
    private:
      std::unique_ptr<uint8_t[]> m_PNG; /**< PNG file data */
      uint32_t m_PNGSize; /**< length of the PNG file data */
  }; //struct
} //namespace

#endif // LIBUHS_HYPERPNGCHUNK_HPP
