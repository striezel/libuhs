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
#include <string>
#include <utility>
#include <vector>
#include "../../libuhs/decryption.hpp"

int main()
{
  std::cout << "Testing old encryption style..." << std::endl;

  /*test data:
        - first elements of pairs are encrypted text
        - second elements of pairs are expected decrypted text
  */
  std::vector<std::pair<std::string, std::string> > data;

  data.push_back(std::pair<std::string, std::string>("gHrGtGs iqrGr",
                                                     "Opening Scene"));
  data.push_back(std::pair<std::string, std::string>("4w{ Bw d ryqpHr CIwv JDr JDzsy",
                                                     "How do I escape from the thugs"));
  data.push_back(std::pair<std::string, std::string>("kD| pIrGSJ JDrIr pG| vwIr DtGJy DrIr",
                                                     "Why aren't there any more hints here"));
  data.push_back(std::pair<std::string, std::string>(":Dr CwFFw{tGs JrLJ {tFF pHHrpI rGqI|HJrB VV tJ ty tGJrGBrB py tGyJIzqJtwGy CwI HrwHFr {tJDwzJ j4i IrpBrIy",
                                                     "The following text will appear encrypted -- it is intended as instructions for people without UHS readers"));
  data.push_back(std::pair<std::string, std::string>(":Dty CtFr Dpy ArrG {ItJJrG CwI p Gr{rI j4i CwIvpJ",
                                                     "This file has been written for a newer UHS format"));

  for (auto&& i: data)
  {
    const std::string plain = libuhs::Decryption::UHS88a(i.first);
    if (plain != i.second)
    {
      std::cout << "Decryption failed!" << std::endl
                << "Encrypted text: " << i.first << std::endl
                << "Decrypted text: " << plain << std::endl
                << "Expected text:  " << i.second << std::endl;
      return 1;
    }
  } //for
  //all is fine
  std::cout << "Decryption test for old encryption passed." << std::endl;
  return 0;
}
