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

#include "file.hpp"
#include "decryption.hpp"
#include "utility.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>

namespace libuhs
{

File::File(const std::string& fileName)
: m_Stream(fileName, std::ios::binary | std::ios::in),
  m_MainTitle(std::string()),
  m_FirstHintLine(0),
  m_LastHintLine(0),
  m_DirectoryTree(std::vector<std::pair<std::string, uint32_t> >()),
  m_HintLines(std::vector<std::string>())
{
  if (!m_Stream.good())
    throw std::ios_base::failure("Could not open file "+fileName+"!");
}

File::~File()
{
  if (m_Stream.is_open())
  {
    m_Stream.close();
  }
}

const std::string& File::getMainTitle() const
{
  return m_MainTitle;
}

const std::vector<std::pair<std::string, uint32_t> >& File::getDirectoryTree() const
{
  return m_DirectoryTree;
}


const std::vector<std::string>& File::getHintLines() const
{
  return m_HintLines;
}

bool File::read()
{
  if (!m_Stream.good())
    return false;

  if (m_Stream.tellg() > 0)
    m_Stream.seekg(0);

  //read header data
  if (!readHeader())
  {
    return false;
  }

  //read directory tree (old format)
  if (!readDirectoryTree())
    return false;

  //read hint lines (old format)
  return readHintLines();
}

bool File::readHeader()
{
  const unsigned int bufferSize = 4096;
  std::unique_ptr<char[]> buffer(new char[bufferSize]);
  std::memset(buffer.get(), 0, bufferSize);

  //read "UHS\r\n"
  m_Stream.read(buffer.get(), 5);
  if (!m_Stream.good())
    return false;
  //Is it a UHS file?
  if (std::string(buffer.get())!="UHS\r\n")
  {
    throw std::runtime_error("File does not have UHS header!");
    return false;
  }


  std::memset(buffer.get(), 0, 6);

  m_Stream.getline(buffer.get(), bufferSize-1, '\n');
  if (!m_Stream.good())
  {
    throw std::runtime_error("Unable to read main title from file!");
    return false;
  }
  m_MainTitle = std::string(buffer.get());
  removeTrailingCarriageReturn(m_MainTitle);

  //read first hint line number
  std::memset(buffer.get(), 0, 4096);
  m_Stream.getline(buffer.get(), bufferSize-1, '\n');
  if (!m_Stream.good())
  {
    throw std::runtime_error("Unable to read number of first hint line number from file!");
    return false;
  }
  std::string tempLine(std::string(buffer.get()));
  removeTrailingCarriageReturn(tempLine);
  if (!stringToUnsignedInt<uint32_t>(tempLine, m_FirstHintLine))
  {
    throw std::runtime_error("\""+tempLine+"\" is not an integer!");
    return false;
  }

  //read last hint line number
  std::memset(buffer.get(), 0, 4096);
  m_Stream.getline(buffer.get(), bufferSize-1, '\n');
  if (!m_Stream.good())
  {
    throw std::runtime_error("Unable to read number of last hint line number from file!");
    return false;
  }
  tempLine = std::string(buffer.get());
  removeTrailingCarriageReturn(tempLine);
  if (!stringToUnsignedInt<uint32_t>(tempLine, m_LastHintLine))
  {
    throw std::runtime_error("\""+tempLine+"\" is not an integer!");
    return false;
  }

  //header part is done here
  return true;
}

bool File::readDirectoryTree()
{
  if (!m_Stream.good())
    return false;

  const unsigned int bufferSize = 4096;
  std::unique_ptr<char[]> buffer(new char[bufferSize]);

  uint32_t i = 1;
  while (i < m_FirstHintLine)
  {
    std::memset(buffer.get(), 0, 4096);
    m_Stream.getline(buffer.get(), bufferSize-1, '\n');
    if (!m_Stream.good())
    {
      throw std::runtime_error("Unable to read next directory tree entry!");
      return false;
    }
    std::string name = std::string(buffer.get());
    removeTrailingCarriageReturn(name);
    name = Decryption::UHS88a(name);

    std::memset(buffer.get(), 0, 4096);
    m_Stream.getline(buffer.get(), bufferSize-1, '\n');
    if (!m_Stream.good())
    {
      throw std::runtime_error("Unable to read number directory tree's hint line from file!");
      return false;
    }
    uint32_t lineNumber = 0;
    std::string tmpLine(std::string(buffer.get()));
    removeTrailingCarriageReturn(tmpLine);
    if (!stringToUnsignedInt<uint32_t>(tmpLine, lineNumber))
    {
      throw std::runtime_error("\"" + tmpLine + "\" is not an integer!");
      return false;
    }
    m_DirectoryTree.push_back(std::pair<std::string, uint32_t>(name, lineNumber));

    //increase line counter by two, because two lines were read
    i = i + 2;
  } //while
  //The current line number should now be the first hint line's number.
  return (i == m_FirstHintLine);
}

bool File::readHintLines()
{
  //Do we even have nice hint line numbers yet?
  if (m_FirstHintLine == 0 || m_LastHintLine == 0
      || m_FirstHintLine > m_LastHintLine)
    return false;

  if (!m_Stream.good())
    return false;
  if (m_Stream.tellg() > 0)
    m_Stream.seekg(0);
  //jump to first hint line (header is four lines, so add three)
  if (!skipLines(m_FirstHintLine+3))
    return false;

  uint32_t currentLine = m_FirstHintLine;

  const unsigned int bufferSize = 4096;
  std::unique_ptr<char[]> buffer(new char[bufferSize]);

  while (currentLine <= m_LastHintLine)
  {
    std::memset(buffer.get(), 0, bufferSize);
    m_Stream.getline(buffer.get(), bufferSize-1, '\n');
    if (!m_Stream.good())
    {
      throw std::runtime_error("Could not read next hint line from file!");
      return false;
    } //if
    std::string tmpLine(std::string(buffer.get()));
    removeTrailingCarriageReturn(tmpLine);
    m_HintLines.push_back(Decryption::UHS88a(tmpLine));
    ++currentLine;
  } //while
  return true;
}

bool File::skipLines(const unsigned int linesToSkip)
{
  return libuhs::skipLines(m_Stream, linesToSkip);
}

} //namespace
