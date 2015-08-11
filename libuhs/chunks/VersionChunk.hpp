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

#ifndef LIBUHS_VERSIONCHUNK_HPP
#define LIBUHS_VERSIONCHUNK_HPP

#include <cstdint>
#include <string>
#include "BasicChunk.hpp"

namespace libuhs
{
  class VersionChunk: public BasicChunk
  {
      std::string m_Version; /**< version string */
      std::string m_additionalText; /**< additional text in version chunk */
    public:
      /** \brief default constructor
       *
       * \param start   starting line of the chunk
       * \param version   version string
       * \param additional  additional text
       */
      VersionChunk(const uint32_t start = 0, const std::string& version=std::string(), const std::string& additional=std::string());


      ///destructor
      ~VersionChunk() {}


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


      /** \brief returns the version string
       *
       * \return Returns the version string.
       * Returns an empty string, if no version was set yet.
       */
      const std::string& getVersion() const;


      /** \brief sets a new version value
       *
       * \param newVersion   the new version
       */
      void setVersion(const std::string& newVersion);


      /** \brief returns the additional text of the version chunk
       *
       * \return Returns the additional text of the version chunk.
       * Returns an empty string, if no additional text was set yet.
       */
      const std::string& additionalText() const;


      /** \brief equality operator
       *
       * \param other   the other version chunk
       * \return Returns true, if the other version chunk is equal to this one.
       */
      bool operator==(const VersionChunk& other) const;


      uint32_t startingLine; /**< starting line of the chunk */
  }; //struct
} //namespace

#endif // LIBUHS_LINKCHUNK_HPP
