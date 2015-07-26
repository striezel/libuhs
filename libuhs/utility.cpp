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

#include "utility.hpp"

namespace libuhs
{

void removeTrailingCarriageReturn(std::string& line)
{
  if (!line.empty())
  {
    // -> remove \r characters at the end, if present
    if (line.at(line.length()-1)=='\r')
    {
      line.erase(line.length()-1);
    }
  } //if
}

} //namespace
