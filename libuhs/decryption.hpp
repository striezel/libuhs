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

#ifndef DECRYPTION_HPP
#define DECRYPTION_HPP

#include <string>

namespace libuhs
{
  struct Decryption
  {
    /** \brief decrypts an old UHS88a-style encrypted string
     *
     * \param message  the encrypted message
     * \return returns the decrypted string
     */
    static std::string UHS88a(const std::string& message);


    /** \brief generates a decryption key based on the given main label
     *
     * \param mainLabel  the main label
     * \return Returns decryption key.
     */
    static std::string generateKey(const std::string& mainLabel);


    /** \brief decrypts a message from a nesthint hunk
     *
     * \param key  the decryption key, obtained from generateKey()
     * \param encryptedText  the encrypted text
     * \return Returns the decrypted text.
     */
    static std::string nesthint(const std::string& key, const std::string& encryptedText);


    /** \brief decrypts a message from a text hunk
     *
     * \param key  the decryption key, obtained from generateKey()
     * \param encryptedText  the encrypted text
     * \return Returns the decrypted text.
     */
    static std::string text(const std::string& key, const std::string& encryptedText);


    /** \brief returns a "harmonized" string that is better readable
     *
     * \param text   a text, usually returned by nesthint() or text() functions of this class
     * \param compressSpaces   if true, the function compresses multiple spaces into one single space
     * \return Returns a harmonized string;
     */
    static std::string harmonize(const std::string& text, const bool compressSpaces = true);
  }; //struct
} //namespace

#endif // DECRYPTION_HPP
