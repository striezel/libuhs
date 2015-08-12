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

#ifndef LIBUHS_FILE_HPP
#define LIBUHS_FILE_HPP

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "chunks/BasicChunk.hpp"

namespace libuhs
{
  struct File
  {
    public:
      /** \brief constructor
       *
       * \param fileName name of the file
       * \remarks Throws an exception, if the given file cannot be opened.
       */
      File(const std::string& fileName);

      //destructor
      ~File();


      /** \brief tries to read the whole file
       *
       * \return Returns true, if read operation was successful.
       *         Returns false, if the read operation failed.
       */
      bool read();


      /** \brief returns the main title of the file (old format)
       *
       * \return returns the main title of the file (old format), if it was
       *         already read. Returns an empty string otherwise.
       */
      const std::string& getMainTitle() const;


      /** \brief returns the directory tree of the file (old format)
       *
       * \return returns the directory tree of the file (old format), if it
       *         was already read. Returns an empty vector otherwise.
       */
      const std::vector<std::pair<std::string, uint32_t> >& getDirectoryTree() const;


      /** \brief returns the hint lines of the file (old format)
       *
       * \return returns the hint lines of the file (old format), if they were
       *         already read. Returns an empty vector otherwise.
       */
      const std::vector<std::string>& getHintLines() const;


      /** \brief chunks that were read from the file
       *
       * \return Returns a vector of chunk pointers.
       */
      const std::vector<std::unique_ptr<BasicChunk> >& getChunks() const;
    private:
      std::ifstream m_Stream;

      std::string m_MainTitle; /**< main title of the file (old format) */
      uint32_t m_FirstHintLine; /**< line number of the first hint */
      uint32_t m_LastHintLine; /**< line number of the last hint */
      std::istream::pos_type m_ChunkStart; /**< file position where the chunks start */
      std::vector<std::unique_ptr<BasicChunk> > m_Chunks; /**< the chunks */

      std::vector<std::pair<std::string, uint32_t> > m_DirectoryTree; /**< directory tree */

      std::vector<std::string> m_HintLines; /**< the hint lines (old format) */

      /** \brief tries to read the file header
       *
       * \param
       * \param
       * \return Returns true, if file header was read.
       *         Returns false, if read operation failed.
       */
      bool readHeader();


      /** \brief tries to read the file's directory tree
       *
       * \return Returns true, if file directory tree was read.
       *         Returns false, if read operation failed.
       */
      bool readDirectoryTree();


      /** \brief tries to read the file's hint lines (old format)
       *
       * \return Returns true, if file directory tree was read.
       *         Returns false, if read operation failed.
       */
      bool readHintLines();


      /** \brief tries to read the new format's chunk data
       *
       * \return Returns true, if the chunk data was read.
       *         Returns false, if read operation failed.
       */
      bool readChunkData();


      /** \brief utility function that skips lines, if needed
       *
       * \param linesToSkip  the number of lines that will be skipped
       * \return Returns true, if the given number of lines could be skipped.
       *         Returns false on failure.
       */
      bool skipLines(const unsigned int linesToSkip);
  }; //struct
} //namespace

#endif // LIBUHS_FILE_HPP
