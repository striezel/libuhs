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

#include <iostream>
#include <stdexcept>
#include <string>
#include "../../libuhs/file.hpp"

const std::vector<std::string> expectedLines =
  {
    "The following text will appear encrypted -- it is intended as instructions",
    "for people without UHS readers.",
    ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::",
    ")QSg MSYK Sg K]G_IKI S] iQK +]SmKegCY pS]i 'sgiK[ M_e[Ci<  )QK +p' Sg",
    "IKgSO]KI i_ gQ_o s_k _]Ys iQK QS]ig iQCi s_k ]KKI g_ s_ke OC[K Sg ]_i",
    "ga_SYKI<  3_k oSYY ]KKI C +p' eKCIKe i_ mSKo iQSg MSYK<  3_k GC] MS]I",
    "+p' eKCIKeg C]I [_eK S]M_e[CiS_] _] iQK +p' Ci QiiaT>>ooo<kQg:QS]ig<G_[>",
    ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::",
    "This file has been written for a newer UHS format which the reader that",
    "you are using does not support.  Visit the UHS web site at",
    "http://www.uhs-hints.com/ to see if a newer reader is available."
  };

const std::vector<std::pair<std::string, uint32_t> > expectedDir =
  {
    std::pair<std::string, uint32_t>("Important!", 3),
    std::pair<std::string, uint32_t>("Ignore this!", 7),
    std::pair<std::string, uint32_t>("Why aren't there any more hints here", 16)
  };

int main(int argc, char** argv)
{
  std::string fileName = "test.dat";
  if (argc>1 && argv != nullptr)
  {
    if (argv[1] != nullptr)
    {
        fileName = std::string(argv[1]);
    }
  } //if

  std::cout << "Trying to read old file \"" << fileName << "\"..." << std::endl;

  try
  {
    libuhs::File uhsFile(fileName);
    if (!uhsFile.read())
    {
      std::cout << "Unable to read file \"" << fileName
                << "\"! The test has failed." << std::endl;
      return 1;
    } //if

    std::cout << "Main title: " << uhsFile.getMainTitle() << std::endl;

    const auto& dirTree = uhsFile.getDirectoryTree();
    for (auto&& i: dirTree)
    {
      std::cout << "Directory entry \""<<i.first << "\" starting at line "
                << i.second << "." << std::endl;
    }
    if (dirTree != expectedDir)
    {
      std::cout << "Error: The directory tree do not match the expected directory!" << std::endl;
      return 1;
    }

    const auto& lines = uhsFile.getHintLines();
    if (lines.size()!=11)
    {
      std::cout << "Error: Expected 11 hint lines, but got " << lines.size()
                << " lines instead." << std::endl;
      return 1;
    }
    std::cout << "File contains " << lines.size()
              << " hint lines, which will follow now:" << std::endl;
    std::cout << "<<<<START OF LINES>>>>" << std::endl;
    for (auto&& i: lines)
    {
      std::cout << i << std::endl;
    }
    std::cout << "<<<<END OF LINES>>>>" << std::endl;

    if (lines != expectedLines)
    {
      std::cout << "Error: The hint lines do not match the expected hint lines!" << std::endl;
      return 1;
    }
  } //try
  catch(std::runtime_error& ex)
  {
    std::cout << "An exception occurred while trying to read \"" << fileName
              << "\"! The test has failed." << std::endl;
    std::cout << "Exception message:" << std::endl
              << ex.what() << std::endl;
    return 1;
  } //try-catch
  catch(std::exception& ex)
  {
    std::cout << "An exception occurred while trying to read \"" << fileName
              << "\"! The test has failed." << std::endl;
    std::cout << "Exception message:" << std::endl
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  return 0;
}
