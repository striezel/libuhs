/*
 -------------------------------------------------------------------------------
    This file is part of the testsuite of libuhs.
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

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "../../libuhs/utility.hpp"
#include "../../libuhs/chunks/InfoChunk.hpp"

int main(int argc, char** argv)
{
  std::string fileName = "info_chunk.dat";
  if (argc>1 && argv != nullptr)
  {
    if (argv[1] != nullptr)
    {
        fileName = std::string(argv[1]);
    }
  } //if

  std::cout << "Trying to read info chunk from \"" << fileName << "\"..." << std::endl;

  std::ifstream inFileStream;
  inFileStream.open(fileName, std::ios_base::in | std::ios_base::binary);
  if (!inFileStream.is_open() || !inFileStream.good())
  {
    std::cout << "Error: Could not open file " << fileName << "!" << std::endl;
    return 1;
  }

  const unsigned int bufferSize = 1024;
  std::unique_ptr<char[]> buffer(new char[bufferSize]);
  std::memset(buffer.get(), 0, bufferSize);

  inFileStream.getline(buffer.get(), bufferSize-1, '\n');
  if (!inFileStream.good())
  {
    std::cout << "Unable to read chunk header from stream!" << std::endl;
    return false;
  }
  std::string line = std::string(buffer.get());
  libuhs::removeTrailingCarriageReturn(line);

  const auto pieces = libuhs::splitAtSeparator(line, ' ');
  if (pieces.size() != 2)
  {
    std::cout << "Error: line contains " << pieces.size()
              << " pieces instead of two!" << std::endl;
    return 1;
  }

  //check if it is a info chunk
  if (pieces[1] != "info")
  {
    std::cout << "Error: expected info chunk, but found \"" << pieces[1]
              << "\" instead!" << std::endl;
    return 1;
  }

  unsigned int linesTotal = 0;
  if (!libuhs::stringToUnsignedInt<unsigned int>(pieces[0], linesTotal))
  {
    std::cout << "Error: \"" << pieces[0] << "\" is not an unsigned integer!" << std::endl;
    return 1;
  }
  if (linesTotal!=16)
  {
    std::cout << "Error: expected line count to be 16, but found \"" << linesTotal
              << "\" instead!" << std::endl;
    return 1;
  }

  libuhs::InfoChunk ic;
  try
  {
    bool success = ic.readFromStream(inFileStream, linesTotal);
    inFileStream.close();
    if (!success)
    {
      std::cout << "Error: Reading info chunk failed!" << std::endl;
      return 1;
    }
    std::cout << "Info: Successfully read info chunk from file!" << std::endl;
    std::cout << "Length: " << ic.fileLength << std::endl
              << "Date: " << ic.date << std::endl
              << "Time: " << ic.time << std::endl
              << "Author: " << ic.author << std::endl
              << "Author note: " << ic.authorNote << std::endl
              << "Publisher: " << ic.publisher << std::endl
              << "Copyright notice: " << ic.copyrightNotice << std::endl
              << "Game note: " << ic.gameNote << std::endl;
    //check if contents match expected contents
    if (ic.fileLength != 123456
      or ic.date != "31-Feb-15"
      or ic.time != "22:23:24"
      or ic.author != "Kermit the Frog"
      or ic.publisher != "The Pub"
      or ic.copyrightNotice != "This is some random copyright notice. It has really not much of a purpose here and is just used for unit testing, in particular for testing InfoChunk::readFromStream()."
      or ic.authorNote != "Kermit the Frog, aka your favourite green guy, can be reached at frog@example.com."
      or ic.gameNote != "This game was published by somebody else.  All trademarks in this file are the property of their respective owners."
       )
    {
      std::cout << "Error: Info chunk contents do not match the expected values!" << std::endl;
      return 1;
    } //if
    std::cout << "Passed test: Contents are exactly as we want them." << std::endl;
  } //try
  catch(std::exception& except)
  {
    std::cout << "Caught exception: " << except.what() << std::endl;
    if (inFileStream.is_open())
      inFileStream.close();
    return 1;
  } //try-catch
  if (inFileStream.is_open())
    inFileStream.close();

  return 0;
}
