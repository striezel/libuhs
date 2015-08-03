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

std::string Decryption::generateKey(const std::string& mainLabel)
{
  //constant "key"
  const std::string k("key");
  /* The real key will have same length as main label, so we can intialize it
   * with main label's value. */
  std::string theKey(mainLabel);
  unsigned int i;
  for (i=0; i<mainLabel.size(); ++i)
  {
    unsigned char currentLetter = mainLabel[i] + (static_cast<unsigned char>(k[i%3]) xor static_cast<unsigned char>(i + 40));
    while (currentLetter>127)
    {
      currentLetter = currentLetter - 96;
    }
    theKey[i] = currentLetter;
  } //for
  return std::move(theKey);
}

std::string Decryption::text(const std::string& key, const std::string& encryptedText)
{
  std::string decryptedText(encryptedText);
  unsigned int i;
  for (i=0; i<encryptedText.size(); ++i)
  {
    const unsigned int codeoffset = i % key.size();
    unsigned char currentLetter = encryptedText[i] - (static_cast<unsigned char>(key[codeoffset]) xor static_cast<unsigned char>(codeoffset + 40));
    while (currentLetter>127)
    {
      currentLetter = currentLetter - 96;
    } //while
    /*
    if (currentLetter == '\0')
      currentLetter = ' ';
    */
    while (currentLetter<32)
    {
      currentLetter = currentLetter + 96;
    } //while
    decryptedText[i] = currentLetter;
    if (currentLetter == 0x60)
    {
      decryptedText[i] = ' ';
    }
  } //for
  return decryptedText;
}

} //namespace
