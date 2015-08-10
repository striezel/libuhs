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

#include "LinkChunk.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include "../utility.hpp"

namespace libuhs
{

LinkChunk::LinkChunk(const uint32_t start, const std::string& lbl, const uint32_t dest)
: BasicChunk(lbl),
  startingLine(start),
  destinationLine(dest)
{
}

ChunkType LinkChunk::getType() const
{
  return ChunkType::link;
}

bool LinkChunk::readFromStream(std::istream& input, const unsigned int linesTotal)
{
  //link chunk has exactly three lines: header, link label, link destination
  if (linesTotal != 3)
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
    throw std::runtime_error("Unable to read chunk label from stream!");
    return false;
  }
  std::string line = std::string(buffer.get());
  removeTrailingCarriageReturn(line);
  setLabel(line);

  //read link destination line
  std::memset(buffer.get(), 0, bufferSize);
  input.getline(buffer.get(), bufferSize-1, '\n');
  if (!input.good())
  {
    throw std::runtime_error("Error: Unable to read link destination line from stream!");
    return false;
  }
  line = std::string(buffer.get());
  removeTrailingCarriageReturn(line);
  if (!stringToUnsignedInt<uint32_t>(line, destinationLine))
  {
    throw std::runtime_error("Error: " + line + " cannot be converted to a link destination number!");
    return false;
  } //if not convertible to unsigned integer
  return true;
}

bool LinkChunk::operator==(const LinkChunk& other) const
{
  return ((startingLine == other.startingLine) && (getLabel() == other.getLabel())
      && (destinationLine == other.destinationLine));
}

} //namespace
