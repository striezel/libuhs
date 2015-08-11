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

#include "VersionChunk.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include "../utility.hpp"

namespace libuhs
{

VersionChunk::VersionChunk(const uint32_t start, const std::string& version, const std::string& additional)
: BasicChunk("version chunk"),
  startingLine(start),
  m_Version(version),
  m_additionalText(additional)
{
}

ChunkType VersionChunk::getType() const
{
  return ChunkType::version;
}

bool VersionChunk::readFromStream(std::istream& input, const unsigned int linesTotal)
{
  //version chunk has at least two lines: header, version number
  if (linesTotal < 2)
    return false;

  if (!input.good())
    return false;

  unsigned int bufferSize = 4096;
  std::unique_ptr<char[]> buffer(new char[bufferSize]);
  std::memset(buffer.get(), 0, bufferSize);
  //read chunk label
  input.getline(buffer.get(), bufferSize-1, '\n');
  if (!input.good())
  {
    throw std::runtime_error("Unable to read version from stream!");
    return false;
  }
  std::string line = std::string(buffer.get());
  removeTrailingCarriageReturn(line);
  m_Version = line;

  m_additionalText.clear();
  uint32_t processedLines = 2;
  while (processedLines < linesTotal)
  {
    //read additional line
    std::memset(buffer.get(), 0, bufferSize);
    input.getline(buffer.get(), bufferSize-1, '\n');
    if (!input.good())
    {
      throw std::runtime_error("Error: Unable to read additonal version line from stream!");
      return false;
    }
    line = std::string(buffer.get());
    removeTrailingCarriageReturn(line);
    if (!m_additionalText.empty())
      m_additionalText += " " + line;
    else
      m_additionalText = line;
    ++processedLines;
  } //while
  return true;
}

const std::string& VersionChunk::getVersion() const
{
  return m_Version;
}

void VersionChunk::setVersion(const std::string& newVersion)
{
  m_Version = newVersion;
}

const std::string& VersionChunk::additionalText() const
{
  return m_additionalText;
}

bool VersionChunk::operator==(const VersionChunk& other) const
{
  return ((startingLine == other.startingLine) && (m_Version == other.getVersion())
      && (m_additionalText == other.m_additionalText));
}

} //namespace
