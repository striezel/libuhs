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

#include "decryption.hpp"

namespace libuhs
{

std::string Decryption::UHS88a(const std::string& message)
{
  const std::string::size_type len = message.size();
  std::string result(message);
  std::string::size_type i = 0;
  for (i=0; i<len; ++i)
  {
    if (message[i]<32)
    {
      //Nothing to do, identical.
      //result[i] = message[i];
    }
    else if (message[i]<80)
    {
      result[i] = 2*message[i]-32;
    }
    else
    {
      result[i] = 2*message[i]-127;
    }
  } //for
  return std::move(result);
}

} //namespace
