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

#ifndef BASICCHUNK_HPP
#define BASICCHUNK_HPP

#include <istream>
#include <string>

namespace libuhs
{
  /** enumeration type for chunk types */
  enum class ChunkType {hint, hyperpng, info, link, nesthint, subject, text, version};

  class BasicChunk
  {
    //private:
      std::string m_Label;
    public:
      /** \brief constructor
       *
       * \param label   the chunk label
       */
      BasicChunk(const std::string& label = "");


      ///virtual destructor
      virtual ~BasicChunk() {}


      /** \brief gets the type of the hunk
       *
       * \return returns the type of the hunk as enumeration
       */
      virtual ChunkType getType() const = 0;


      /** \brief retrieves the chunk label
       *
       * \return Returns the current chunk label
       */
      const std::string& getLabel() const;


      /** \brief sets a new chunk label
       *
       * \param newLabel   the new chunk label
       */
      void setLabel(const std::string& newLabel);


      /** \brief tries to read the rest of the chunk from the given stream
       *
       * \param input   input stream
       * \param linesTotal  number of total lines in this hunk
       * \return Returns true, if chunk was read successfully.
       *         Returns false, if read operation failed.
       */
      virtual bool readFromStream(std::istream& input, const unsigned int linesTotal) = 0;
  }; //struct

} //namespace

#endif // BASICCHUNK_HPP
