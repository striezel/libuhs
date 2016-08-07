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

#include "InfoChunk.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include "../utility.hpp"

namespace libuhs
{

InfoChunk::InfoChunk()
: BasicChunk(),
  fileLength(0),
  date(""),
  time(""),
  author(""),
  publisher(""),
  copyrightNotice(""),
  authorNote(""),
  gameNote("")
{
}

ChunkType InfoChunk::getType() const
{
  return ChunkType::info;
}

void InfoChunk::reset()
{
  fileLength = 0;
  date.clear();
  time.clear();
  author.clear();
  publisher.clear();
  copyrightNotice.clear();
  authorNote.clear();
  gameNote.clear();
}

bool InfoChunk::readFromStream(std::istream& input, const unsigned int linesTotal)
{
  if (linesTotal<10)
    return false;

  if (!input.good())
    return false;

  //clear all existing data in this chunk
  reset();

  const unsigned int bufferSize = 4096;
  std::unique_ptr<char[]> buffer(new char[bufferSize]);

  unsigned int processedLines = 1;
  while (processedLines<linesTotal)
  {
    std::memset(buffer.get(), 0, bufferSize);
    //read next line
    input.getline(buffer.get(), bufferSize-1, '\n');
    if (!input.good())
    {
      throw std::runtime_error("Unable to read next line of info chunk from stream!");
      return false;
    }
    std::string line = std::string(buffer.get());
    removeTrailingCarriageReturn(line);
    const std::string::size_type separatorPos = line.find('=');
    if (separatorPos != std::string::npos)
    {
      const std::string key = line.substr(0, separatorPos);
      const std::string value = line.substr(separatorPos+1);
      if (key == "length")
      {
        if (!stringToUnsignedInt(value, fileLength))
        {
          throw std::runtime_error("Error while processing info chunk: "+value+" is not a valid unsigned integer!");
          return false;
        } //if conversion failed
      } //if length
      else if (key == "date")
        date = value;
      else if (key == "time")
        time = value;
      else if (key == "author")
        author = value;
      else if (key == "publisher")
        publisher = value;
      else if (key == "copyright")
      {
        if (!copyrightNotice.empty())
          copyrightNotice += " " + value;
        else
          copyrightNotice = value;
      } //if copyright-note
      else if (key == "author-note")
      {
        if (!authorNote.empty())
          authorNote += " " + value;
        else
          authorNote = value;
      } //if author note
      else if (key == "game-note")
      {
        if (!gameNote.empty())
          gameNote += " " + value;
        else
          gameNote = value;
      } //if game note
      else
      {
        //unknown key
        throw std::runtime_error("Error: Info chunk contains invalid key \"" + key +"\"!");
        return false;
      }
    } //if '=' was found
    //increase line count
    ++processedLines;
  } //while
  // All lines were read, done. :)
  return true;
}

} //namespace
