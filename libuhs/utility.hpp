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

#ifndef LIBUHS_UTILITY_HPP
#define LIBUHS_UTILITY_HPP

#include <istream>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace libuhs
{

/** \brief tries to convert a string to an unsigned integer value
 *
 * \param str  the string that may possibly contain an integer
 * \param value  the integer variable that will be used to store the value
 * \return Returns true, if the string could be converted to uintT.
 *         Returns false otherwise.
 */
template<typename uintT>
bool stringToUnsignedInt(const std::string& str, uintT& value)
{
  static_assert(std::is_integral<uintT>::value, "Template type uintT must be integral type!");
  static_assert(std::is_unsigned<uintT>::value, "Template type uintT must be unsigned!");

  if (str.empty()) return false;
  value = 0;
  const uintT cTenthLimit = std::numeric_limits<uintT>::max() / 10;
  const uintT cRealLimit = std::numeric_limits<uintT>::max();
  std::string::size_type i = 0;
  for ( ; i < str.size(); ++i)
  {
    if ((str.at(i)>='0') and (str.at(i)<='9'))
    {
      /* If the result of the multiplication in the next line would go out of
         the type range, then the result is not useful anyway, so quit here. */
      if (value>cTenthLimit) return false;
      value = value * 10;
      /* If the result of the addition in the next line would go out of the
         type's range, then the result is not useful anyway, so quit here. */
      if (value>cRealLimit-(str.at(i)-'0')) return false;
      value = value + (str.at(i)-'0');
    }//if
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for
  return true;
}


/** \brief tries to convert an integer value to its string representation
 *
 * \param value  the integer variable that shall be converted
 * \return Returns the string representation of the integer value.
 */
template<typename intT>
std::string intToString(const intT value)
{
  static_assert(std::is_integral<intT>::value, "Template type intT shall be integral type!");

  std::stringstream s_str;
  s_str << value;
  return s_str.str();
}


/** \brief removes a trailing carriage return character from a string
 *
 * \param line  the string that may have a trailing carriage return character
 */
void removeTrailingCarriageReturn(std::string& line);


/** \brief removes a trailing line feed character from a string
 *
 * \param line  the string that may have a trailing line feed character
 */
void removeTrailingLineFeed(std::string& line);


/** \brief splits the given string into pieces, treating a given character
 * as the separator for values, and returns the pieces as a vector
 *
 * \param line  string containing the separated values
 * \param separator  character that serves as separator between elements
 */
std::vector<std::string> splitAtSeparator(std::string line, const char separator);


/** \brief utility function that skips lines in the given stream
 *
 * \param inStream     the input stream where lines shall be skipped
 * \param linesToSkip  the number of lines that will be skipped
 * \return Returns true, if the given number of lines could be skipped.
 *         Returns false on failure.
 */
bool skipLines(std::istream& inStream, const unsigned int linesToSkip);

} //namespace

#endif // LIBUHS_UTILITY_HPP
