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

#include "SubjectChunk.hpp"
#include <cstring>
#include <stdexcept>
#include "../ChunkReader.hpp"
#include "../decryption.hpp"
#include "../utility.hpp"

namespace libuhs
{

SubjectChunk::SubjectChunk(const uint32_t start, const std::string& lbl, const std::string& key)
: BasicChunk(lbl),
  startingLine(start),
  decryptionKey(key),
  chunks(std::vector<std::unique_ptr<BasicChunk> >())
{
}

ChunkType SubjectChunk::getType() const
{
  return ChunkType::subject;
}

bool SubjectChunk::readFromStream(std::istream& input, const unsigned int linesTotal)
{
  //subject chunk has at least three lines: header, subject label, chunk data
  if (linesTotal < 3)
    return false;

  if (!input.good())
    return false;

  //read chunk label
  std::string label;
  std::getline(input, label, '\n');
  if (!input.good())
  {
    throw std::runtime_error("Unable to read chunk label from stream!");
    return false;
  }
  removeTrailingCarriageReturn(label);
  setLabel(label);

  //clear old chunks
  chunks.clear();

  if (decryptionKey.empty())
    decryptionKey = Decryption::generateKey(label);

  if (!ChunkReader::read(input, linesTotal - 2, startingLine+2, decryptionKey, chunks))
    return false;

  //all done
  return true;
}

} //namespace
