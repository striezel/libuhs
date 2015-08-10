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

#include "HyperPNGChunk.hpp"
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include "../utility.hpp"

namespace libuhs
{

// set maximum file size to 2 MB
const uint32_t HyperPNGChunk::MaximumFileSize = 2 * 1024 *1024;

HyperPNGChunk::HyperPNGChunk(const std::string& label)
: BasicChunk(label),
  m_PNG(nullptr),
  m_PNGSize(0)
{
}

ChunkType HyperPNGChunk::getType() const
{
  return ChunkType::hyperpng;
}

bool HyperPNGChunk::readFromStream(std::istream& input, const unsigned int linesTotal)
{
  //HyperPNG chunk has at least three lines: header line, label, offset+length line
  if (linesTotal < 3)
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

  //read file offset and length
  std::memset(buffer.get(), 0, bufferSize);
  input.getline(buffer.get(), bufferSize-1, '\n');
  if (!input.good())
  {
    throw std::runtime_error("Error: Unable to read hyperpng offset line from stream!");
    return false;
  }
  line = std::string(buffer.get());
  removeTrailingCarriageReturn(line);
  const auto pieces = splitAtSeparator(line, ' ');
  if (pieces.size() != 3)
  {
    throw std::runtime_error("Error: " + line + " in hyperpng should contain three numbers!");
    return false;
  } //if piece count does not match

  uint32_t offset = 0;
  uint32_t length = 0;
  if (!stringToUnsignedInt<uint32_t>(pieces[1], offset))
  {
    throw std::runtime_error("Error: " + pieces[1] + " in hyperpng chunk is not an integer value!");
    return false;
  } //if not convertible to unsigned integer
  if (!stringToUnsignedInt<uint32_t>(pieces[2], length))
  {
    throw std::runtime_error("Error: " + pieces[2] + " in hyperpng chunk is not an integer value!");
    return false;
  } //if not convertible to unsigned integer


  const std::istream::pos_type currentPosition = input.tellg();
  if (currentPosition == std::istream::pos_type(-1))
  {
    throw std::runtime_error("Error in hyperpng chunk: Could not save current stream position before jump!");
    return false;
  }
  //jump to offset
  input.seekg(offset);
  if (!input.good())
  {
    //seekg() failed!
    throw std::runtime_error("Error in hyperpng chunk: First seek operation failed!");
    return false;
  }
  //make sure that the length value is somewhat reasonable
  if (length > MaximumFileSize)
  {
    throw std::runtime_error("Error in hyperpng chunk: Length of " +intToString<uint32_t>(length)+ " bytes exceeds the maximum of "
                             + intToString<uint32_t>(MaximumFileSize) + " bytes!");
    return false;
  }
  m_PNG.reset(new uint8_t[length]);
  m_PNGSize = length;

  //read data into buffer
  input.read(reinterpret_cast<char*>(m_PNG.get()), length);
  if (!input.good())
  {
    //read() failed!
    m_PNG.reset(nullptr);
    m_PNGSize = 0;
    throw std::runtime_error("Error in hyperpng chunk: Could not read PNG data!");
    return false;
  }
  //make sure we read the whole data
  if (input.gcount() != length)
  {
    //File probably ended before length bytes were read.
    m_PNG.reset(nullptr);
    m_PNGSize = 0;
    throw std::runtime_error("Error in hyperpng chunk: Could not read the full PNG data!");
    return false;
  }
  //jump back to original position of chunk
  input.seekg(currentPosition);
  if (!input.good())
  {
    //seekg() failed!
    throw std::runtime_error("Error in hyperpng chunk: Second seek operation failed!");
    return false;
  }

  /* Skip remaining lines of hyperpng chunk, if any, because we do not care
     about them yet. */
  unsigned int i;
  for (i=0; i<linesTotal-3; ++i)
  {
    input.getline(buffer.get(), bufferSize-1, '\n');
    if (!input.good())
      return false;
  } //for
  return true;
}

bool HyperPNGChunk::writePNGFile(const std::string& fileName) const
{
  /* File name must not be empty. PNG buffer must not be NULL and have a size
     not equal to zero. */
  if (fileName.empty() || (m_PNGSize == 0) || (m_PNG.get() == nullptr))
    return false;

  std::ofstream outfile(fileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!outfile.good() || !outfile.is_open())
  {
    if (outfile.is_open())
      outfile.close();
    return false;
  } //if file could not be created or opened
  //Write all data to the file stream.
  outfile.write(reinterpret_cast<const char*>(m_PNG.get()), m_PNGSize);
  if (!outfile.good())
  {
    outfile.close();
    return false;
  }
  if (outfile.tellp() < m_PNGSize)
  {
    //Not all data was written.
    outfile.close();
    return false;
  }
  outfile.close();
  return true;
}

uint32_t HyperPNGChunk::getPNGSize() const
{
  return m_PNGSize;
}

} //namespace
