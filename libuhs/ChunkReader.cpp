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

#include "ChunkReader.hpp"
#include <stdexcept>
#include "utility.hpp"
#include "chunks/HintChunk.hpp"
#include "chunks/HyperPNGChunk.hpp"
#include "chunks/InfoChunk.hpp"
#include "chunks/LinkChunk.hpp"
#include "chunks/NestHintChunk.hpp"
#include "chunks/SubjectChunk.hpp"
#include "chunks/TextChunk.hpp"
#include "chunks/VersionChunk.hpp"

namespace libuhs
{

bool ChunkReader::read(std::istream& input, const uint32_t maxLines, const uint32_t lineOffset, const std::string& key, std::vector<std::unique_ptr<BasicChunk> >& chunks)
{
  if (!input.good())
    return false;

  //chunks with less than two lines do not exist
  if (maxLines<2)
    return false;

  uint32_t processedLines = 0;
  while (processedLines < maxLines)
  {
    std::string line("");
    std::getline(input, line, '\n');
    if (!input.good())
    {
      throw std::runtime_error("Error: Unable to read next sub-chunk header of subject chunk from stream!");
      return false;
    } //if
    removeTrailingCarriageReturn(line);
    const auto pieces = splitAtSeparator(line, ' ');
    if (pieces.size() != 2)
    {
      throw std::runtime_error("Invalid chunk header in ChunkReader!");
      return false;
    }
    uint32_t subChunkLines = 0;
    if (!stringToUnsignedInt<uint32_t>(pieces[0], subChunkLines))
    {
      throw std::runtime_error("Invalid sub-chunk header! Line count is not an integer.");
      return false;
    }
    if (subChunkLines <= 0)
    {
      throw std::runtime_error("Invalid sub-chunk header! Line count is zero.");
      return false;
    } //if line count too low


    if (pieces[1] == "hint")
    {
      std::unique_ptr<BasicChunk> subchunk(new HintChunk(lineOffset+processedLines, "", { }));
      if (! subchunk->readFromStream(input, subChunkLines))
      {
        throw std::runtime_error("Failed to read hint sub-chunk!");
        return false;
      }
      chunks.push_back(std::move(subchunk));
      processedLines += subChunkLines;
    }
    else if (pieces[1] == "hyperpng")
    {
      std::unique_ptr<BasicChunk> subchunk(new HyperPNGChunk);
      if (! subchunk->readFromStream(input, subChunkLines))
      {
        throw std::runtime_error("Failed to read link sub-chunk!");
        return false;
      }
      chunks.push_back(std::move(subchunk));
      processedLines += subChunkLines;
    }
    else if (pieces[1] == "link")
    {
      std::unique_ptr<BasicChunk> subchunk(new LinkChunk(lineOffset+processedLines));
      if (! subchunk->readFromStream(input, subChunkLines))
      {
        throw std::runtime_error("Failed to read link sub-chunk!");
        return false;
      }
      chunks.push_back(std::move(subchunk));
      processedLines += subChunkLines;
    }
    else if (pieces[1] == "nesthint")
    {
      std::unique_ptr<BasicChunk> subchunk(new NestHintChunk(lineOffset+processedLines, "", key, { }));
      if (! subchunk->readFromStream(input, subChunkLines))
      {
        throw std::runtime_error("Failed to read nesthint sub-chunk!");
        return false;
      }
      chunks.push_back(std::move(subchunk));
      processedLines += subChunkLines;
    }
    else if (pieces[1] == "subject")
    {
      std::unique_ptr<BasicChunk> subchunk(new SubjectChunk(lineOffset+processedLines, "", key));
      if (! subchunk->readFromStream(input, subChunkLines))
      {
        throw std::runtime_error("Failed to read subject sub-chunk!");
        return false;
      }
      chunks.push_back(std::move(subchunk));
      processedLines += subChunkLines;
    }
    else if (pieces[1] == "text")
    {
      std::unique_ptr<BasicChunk> subchunk(new TextChunk(lineOffset+processedLines, "", key, 0, 0));
      if (! subchunk->readFromStream(input, subChunkLines))
      {
        throw std::runtime_error("Failed to read text sub-chunk!");
        return false;
      }
      chunks.push_back(std::move(subchunk));
      processedLines += subChunkLines;
    }
    else if (pieces[1] == "version")
    {
      std::unique_ptr<BasicChunk> subchunk(new VersionChunk(lineOffset+processedLines, "", ""));
      if (! subchunk->readFromStream(input, subChunkLines))
      {
        throw std::runtime_error("Failed to read version sub-chunk!");
        return false;
      }
      chunks.push_back(std::move(subchunk));
      processedLines += subChunkLines;
    }
    else if (pieces[1] == "info")
    {
      std::unique_ptr<BasicChunk> subchunk(new InfoChunk);
      if (! subchunk->readFromStream(input, subChunkLines))
      {
        throw std::runtime_error("Failed to read info sub-chunk!");
        return false;
      }
      chunks.push_back(std::move(subchunk));
      processedLines += subChunkLines;
      //Info chunk seems to be last chunk in file, so stop after that.
      return true;
    }
    else if ((pieces[1] == "blank") || (pieces[1] == "incentive"))
    {
      //"blank" and "incentive" chunks are empty chunks
      if (subChunkLines >=1)
      {
        if (!skipLines(input, subChunkLines-1))
        {
          throw std::runtime_error("Failed to skip blank/incentive sub-chunk!");
          return false;
        }
        processedLines += subChunkLines;
      }
    } //if blank
    else
    {
      //unknown chunk type
      throw std::runtime_error("Unknown sub-chunk type: " + pieces[1] + "!");
      return false;
    }
  }//while
  return true;
}

} //namespace
