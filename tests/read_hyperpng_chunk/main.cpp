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
#include "../../libuhs/chunks/HyperPNGChunk.hpp"

int main(int argc, char** argv)
{
  std::string fileName = "hyperpng_chunk.dat";
  std::string pngname = "test.png";

  if (argc < 3)
  {
    std::cout << "Error: Not enough parameters!" << std::endl
              << "  1st parameter should be chunk file." << std::endl
              << "  2nd parameter should be original PNG file." << std::endl;
    return 1;
  }
  if (argv != nullptr)
  {
    if (argv[1] != nullptr)
    {
      fileName = std::string(argv[1]);
    }
    if (argv[2] != nullptr)
    {
      pngname = std::string(argv[2]);
    }
  } //if

  std::cout << "Trying to read hyperpng chunk from " << fileName
            << " and compare it with " << pngname << "..." << std::endl;

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

  //check if it is a hyperpng chunk
  if (pieces[1] != "hyperpng")
  {
    std::cout << "Error: expected hyperpng chunk, but found \"" << pieces[1]
              << "\" instead!" << std::endl;
    return 1;
  }

  unsigned int linesTotal = 0;
  if (!libuhs::stringToUnsignedInt<unsigned int>(pieces[0], linesTotal))
  {
    std::cout << "Error: \"" << pieces[0] << "\" is not an unsigned integer!" << std::endl;
    return 1;
  }
  if (linesTotal<3)
  {
    std::cout << "Error: expected line count to be at least three (3), but found \"" << linesTotal
              << "\" instead!" << std::endl;
    return 1;
  }

  libuhs::HyperPNGChunk pngchunk;
  try
  {
    bool success = pngchunk.readFromStream(inFileStream, linesTotal);
    inFileStream.close();
    if (!success)
    {
      std::cout << "Error: Reading hyperpng chunk failed!" << std::endl;
      return 1;
    }
    std::cout << "Info: Successfully read hyperpng chunk from file!" << std::endl;
    const std::string extractionPath = "/tmp/extracted.png";
    if (!pngchunk.writePNGFile(extractionPath))
    {
      std::cout << "Error: Could not write PNG file to " << extractionPath << "!" << std::endl;
      return 1;
    }

    //open original PNG file
    inFileStream.open(pngname, std::ios_base::in | std::ios_base::binary);
    if (!inFileStream.is_open() || !inFileStream.good())
    {
      std::cout << "Error: Could not open file " << pngname << "!" << std::endl;
      return 1;
    }
    std::unique_ptr<char[]> expectedBuffer(new char[pngchunk.getPNGSize()]);
    inFileStream.read(expectedBuffer.get(), pngchunk.getPNGSize());
    if (!inFileStream.good() || inFileStream.gcount() != pngchunk.getPNGSize())
    {
      std::cout << "Error: Could not read all data from " << pngname << "!" << std::endl;
      return 1;
    }
    inFileStream.close();

    //open extracted PNG file
    inFileStream.open(extractionPath, std::ios_base::in | std::ios_base::binary);
    if (!inFileStream.is_open() || !inFileStream.good())
    {
      std::cout << "Error: Could not open file " << extractionPath << "!" << std::endl;
      return 1;
    }
    std::unique_ptr<char[]> extractedBuffer(new char[pngchunk.getPNGSize()]);
    inFileStream.read(extractedBuffer.get(), pngchunk.getPNGSize());
    if (!inFileStream.good() || inFileStream.gcount() != pngchunk.getPNGSize())
    {
      std::cout << "Error: Could not read all data from " << extractionPath << "!" << std::endl;
      return 1;
    }
    inFileStream.close();

    if (std::memcmp(expectedBuffer.get(), extractedBuffer.get(), pngchunk.getPNGSize()) != 0)
    {
      std::cout << "Error: Extracted PNG file and expected PNG file data do not match!" << std::endl;
      return 1;
    }
    else
    {
      std::cout << "Info: PNG data matches the expected data." << std::endl;
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
