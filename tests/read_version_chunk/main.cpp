/*
 -------------------------------------------------------------------------------
    This file is part of the testsuite of libuhs.
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

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "../../libuhs/utility.hpp"
#include "../../libuhs/chunks/VersionChunk.hpp"


const libuhs::VersionChunk cExpected(1, "123a", "This is some additional text.");


int main(int argc, char** argv)
{
  std::string fileName = "version_chunk.dat";
  if (argc>1 && argv != nullptr)
  {
    if (argv[1] != nullptr)
    {
        fileName = std::string(argv[1]);
    }
  } //if

  std::cout << "Trying to read version chunk from \"" << fileName << "\"..." << std::endl;

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

  //check if it is a version chunk
  if (pieces[1] != "version")
  {
    std::cout << "Error: expected version chunk, but found \"" << pieces[1]
              << "\" instead!" << std::endl;
    return 1;
  }

  unsigned int linesTotal = 0;
  if (!libuhs::stringToUnsignedInt<unsigned int>(pieces[0], linesTotal))
  {
    std::cout << "Error: \"" << pieces[0] << "\" is not an unsigned integer!" << std::endl;
    return 1;
  }
  if (linesTotal != 4)
  {
    std::cout << "Error: expected line count to be four (4), but found \"" << linesTotal
              << "\" instead!" << std::endl;
    return 1;
  }

  libuhs::VersionChunk vc(1, "", "");
  try
  {
    bool success = vc.readFromStream(inFileStream, linesTotal);
    inFileStream.close();
    if (!success)
    {
      std::cout << "Error: Reading version chunk failed!" << std::endl;
      return 1;
    }
    std::cout << "Info: Successfully read version chunk from file!" << std::endl;
    std::cout << "  Version: " << vc.getVersion() << std::endl
              << "  Additional text: " << vc.additionalText() << std::endl;

    if (vc == cExpected)
    {
      std::cout << "Version chunk data matches the expected result." << std::endl;
    }
    else
    {
      std::cout << "Error: Version chunk data does NOT match the expected result." << std::endl;
      return 1;
    }
  }
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
