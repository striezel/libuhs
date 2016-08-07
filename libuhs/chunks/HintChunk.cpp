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

#include "HintChunk.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include "../decryption.hpp"
#include "../utility.hpp"

namespace libuhs
{

HintChunk::HintChunk(const uint32_t start, const std::string& lbl, const std::vector<std::string>& theHints)
: BasicChunk(lbl),
  startingLine(start),
  hints(theHints)
{
}

ChunkType HintChunk::getType() const
{
  return ChunkType::hint;
}

bool HintChunk::readFromStream(std::istream& input, const unsigned int linesTotal)
{
  //valid chunk should have at least three lines: chunk header, label, hint line
  if (linesTotal<3)
    return false;

  if (!input.good())
    return false;

  const unsigned int bufferSize = 4096;
  std::unique_ptr<char[]> buffer(new char[bufferSize]);
  std::memset(buffer.get(), 0, bufferSize);

  input.getline(buffer.get(), bufferSize-1, '\n');
  if (!input.good())
  {
    throw std::runtime_error("Unable to read chunk label from stream!");
    return false;
  }
  std::string line = std::string(buffer.get());
  removeTrailingCarriageReturn(line);
  setLabel(line);

  // "-" is hint separator
  const std::string cHintSeparator("-");

  //two lines were processed: header (implicitly) + label (explicitly)
  unsigned int processedLines = 2;

  //reset hints
  hints = {""};

  while (processedLines<linesTotal)
  {
    std::memset(buffer.get(), 0, bufferSize);
    input.getline(buffer.get(), bufferSize-1, '\n');
    if (!input.good())
    {
      throw std::runtime_error("Unable to read hint line from stream!");
      return false;
    }

    line = std::string(buffer.get());
    removeTrailingCarriageReturn(line);

    if (line==cHintSeparator)
    {
      //next hint follows, push empty string into vector
      hints.push_back(std::string());
    } //if
    else
    {
      //decrypt line
      line = Decryption::UHS88a(line);
      if (!hints.back().empty())
      {
        hints.back() = hints.back() + " " + line;
      }
      else
      {
        hints.back() = line;
      }
    } //else
    ++processedLines;
  } //while
  return true;
}

bool HintChunk::operator==(const HintChunk& other) const
{
  return ((startingLine == other.startingLine) && (getLabel() == other.getLabel())
      && (hints == other.hints));
}

} //namespace
