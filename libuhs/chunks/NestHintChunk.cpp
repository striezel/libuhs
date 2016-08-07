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

#include "NestHintChunk.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include "../decryption.hpp"
#include "../utility.hpp"

namespace libuhs
{

NestHintChunk::NestHintChunk(const uint32_t start, const std::string& lbl, const std::string& key, const std::vector<std::string>& theHints)
: BasicChunk(lbl),
  startingLine(start),
  decryptionKey(key),
  hints(theHints)
{
}

ChunkType NestHintChunk::getType() const
{
  return ChunkType::nesthint;
}

bool NestHintChunk::readFromStream(std::istream& input, const unsigned int linesTotal)
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
  std::string label = std::string(buffer.get());
  removeTrailingCarriageReturn(label);
  setLabel(label);

  // "-" is hint separator
  const std::string cHintSeparator("-");
  // "=" is nested hint separator
  const std::string cNestHintSeparator("=");

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
      throw std::runtime_error("Unable to read nested hint line from stream!");
      return false;
    }

    std::string line(buffer.get());
    removeTrailingCarriageReturn(line);

    if (line==cHintSeparator)
    {
      //next hint follows, push empty string into vector
      hints.push_back(std::string());
    } //if hint separator
    else if (line == cNestHintSeparator)
    {
      //Nested chunk follows, let's read it.
      //clear line buffer
      std::memset(buffer.get(), 0, bufferSize);
      input.getline(buffer.get(), bufferSize-1, '\n');
      if (!input.good())
      {
        throw std::runtime_error("Unable to read nested hint line from stream!");
        return false;
      }
      std::string nestedLine(buffer.get());
      removeTrailingCarriageReturn(nestedLine);
      const auto pieces = splitAtSeparator(nestedLine, ' ');
      if (pieces.size() != 2)
      {
        throw std::runtime_error("Invalid chunk header in nested hint line!");
        return false;
      }
      uint32_t nestedChunkLines = 0;
      if (!stringToUnsignedInt<uint32_t>(pieces[0], nestedChunkLines))
      {
        throw std::runtime_error("Invalid chunk header in nested hint line! Line count is not an integer.");
        return false;
      }
      if (nestedChunkLines <= 0)
      {
        throw std::runtime_error("Invalid chunk header in nested hint line! Line count is zero.");
        return false;
      } //if line count too low

      if (!skipLines(input, nestedChunkLines-1))
      {
        throw std::runtime_error("Invalid chunk in nested hint! Lines could not be skipped.");
        return false;
      }
      hints.push_back("Info: Skipped a chunk of type " + pieces[1] + ".");
      hints.push_back("");
      processedLines += nestedChunkLines;
    } //else if nested hint separator
    else
    {
      if (!hints.back().empty())
      {
        hints.back() = hints.back() + " " + Decryption::nesthint(decryptionKey, line);
      }
      else
      {
        hints.back() = Decryption::nesthint(decryptionKey, line);
      }
    } //else
    ++processedLines;
  } //while

  return true;
}

} //namespace
