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

#include "TextChunk.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include "../utility.hpp"
#include "../decryption.hpp"

namespace libuhs
{

TextChunk::TextChunk(const uint32_t start, const std::string& lbl, const std::string& key, const uint32_t _offset, const uint32_t len)
: BasicChunk(lbl),
  startingLine(start),
  decryptionKey(key),
  offset(_offset),
  length(len),
  text("")
{
}

ChunkType TextChunk::getType() const
{
  return ChunkType::text;
}

bool TextChunk::readFromStream(std::istream& input, const unsigned int linesTotal)
{
  //valid chunk should have excatly three lines: chunk header, label, "coordinates"
  if (linesTotal!=3)
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

  //read "coordinates"
  std::memset(buffer.get(), 0, bufferSize);
  input.getline(buffer.get(), bufferSize-1, '\n');
  if (!input.good())
  {
    throw std::runtime_error("Unable to read offset numbers of text chunk from stream!");
    return false;
  }
  line = std::string(buffer.get());
  removeTrailingCarriageReturn(line);
  const auto pieces = splitAtSeparator(line, ' ');
  if (pieces.size() != 4)
  {
    throw std::runtime_error("Text chunk is expected to have exactly four values in third line, but found "
                             + intToString<unsigned int>(pieces.size()) + " instead!");
    return false;
  }
  offset = 0;
  length = 0;
  if (!stringToUnsignedInt<unsigned int>(pieces[2], offset))
  {
    throw std::runtime_error("Error in text chunk: " + pieces[2] + " is not an integer value!");
    return false;
  }
  if (!stringToUnsignedInt<unsigned int>(pieces[3], length))
  {
    throw std::runtime_error("Error in text chunk: " + pieces[3] + " is not an integer value!");
    return false;
  }

  const std::istream::pos_type currentPosition = input.tellg();
  if (currentPosition == std::istream::pos_type(-1))
  {
    throw std::runtime_error("Error in text chunk: Could not save current stream position before jump!");
    return false;
  }
  //jump to offset
  input.seekg(offset);
  if (!input.good())
  {
    //seekg() failed!
    throw std::runtime_error("Error in text chunk: First seek operation failed!");
    return false;
  }
  if (length > bufferSize-1)
  {
    if (length > 1024*1024)
    {
      throw std::runtime_error("Error in text chunk: Length is more than 1 MB!");
      return false;
    }
    buffer.reset(new char[length+1]);
    bufferSize = length+1;
  } //if re-allocation of larger buffer is required
  std::memset(buffer.get(), 0, bufferSize);
  input.read(buffer.get(), length);
  if (!input.good())
  {
    throw std::runtime_error("Error in text chunk: Could not read encrypted text!");
    return false;
  }
  std::string encryptedText(buffer.get());
  while (encryptedText.size() < length)
  {
    encryptedText.push_back('\0');
    encryptedText += std::string(buffer.get()+encryptedText.length());
  } //while
  removeTrailingLineFeed(encryptedText);
  removeTrailingCarriageReturn(encryptedText);
  text = Decryption::text(decryptionKey, encryptedText);
  //jump back to original position of chunk
  input.seekg(currentPosition);
  if (!input.good())
  {
    //seekg() failed!
    throw std::runtime_error("Error in text chunk: Second seek operation failed!");
    return false;
  }
  return true;
}

} //namespace
