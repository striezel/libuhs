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
#include "../../libuhs/chunks/HintChunk.hpp"


const libuhs::HintChunk cExpected = {
    1, std::string("Warrior"),
    {
        std::string("Often, the simplest and most effective way to deal with a problem in Arx is to stick a sword into it.")
        +" There is much to be said in favor of the brutish approach: melee weapons in Arx can inflict severe damage, are faster and easier to wield than the complex runes required for magic spells, and are the preferred fighting style of most of the enemies you will encounter.",
        "The most important skill for this type of character is Close Combat. This skill reflects your ability to wield weapons effectively, and grants a bonus to the damage inflicted upon a successful hit. It also increases your chances of scoring a critical hit for double damage. Additionally, most of the better weapons in the game require a certain level of skill in Close Combat before you can use them, so allotting more points to Close Combat will give you access to more powerful weaponry.",
        "Because you will be fighting most battles at close range, it is important to put some points into the Defense skill as well. A high rating in Defense results in an improved armor class and a stronger resistance to the effects of poison. This skill is also used to determine your effectiveness in blocking attacks using a shield.",
        "There will be times when you need to maintain a safe distance from your opponent, or wish to soften him up before closing in for the kill. A few points in Projectile skill will allow you to use a bow accurately. Alternatively, you may choose to use magic in these situations, and in fact it is not recommended that you neglect the Casting skill completely in favor of melee attacks, as there are several enemies in the game which are difficult if not impossible to defeat without magic spells. A small investment in this skill will at least give you the ability to rip off a decent Fireball spell when the need arises.",
        "Other skills which may be useful to a warrior-type include Object Knowledge, for mixing potions of healing (often desperately needed during the heat of battle), repairing weapons and armor, and imbuing weapons with special enhancements such as poison. You may find a few points spent on the Stealth skill helpful as well, to give you the element of surprise when confronting an enemy.",
        "When adding points to your character's attributes, pay particular attention to Strength, which will allow you to deal more damage and is a requirement for equipping certain weapons; and Dexterity, which improves your speed and accuracy in combat and increases your chances of scoring a critical hit. You'll probably want to invest some points into Constitution as well, to increase your hit point total."
    }
};

int main(int argc, char** argv)
{
  std::string fileName = "hint_chunk.dat";
  if (argc>1 && argv != nullptr)
  {
    if (argv[1] != nullptr)
    {
        fileName = std::string(argv[1]);
    }
  } //if

  std::cout << "Trying to read hint chunk from \"" << fileName << "\"..." << std::endl;

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

  //check if it is a hint chunk
  if (pieces[1] != "hint")
  {
    std::cout << "Error: expected hint chunk, but found \"" << pieces[1]
              << "\" instead!" << std::endl;
    return 1;
  }

  unsigned int linesTotal = 0;
  if (!libuhs::stringToUnsignedInt<unsigned int>(pieces[0], linesTotal))
  {
    std::cout << "Error: \"" << pieces[0] << "\" is not an unsigned integer!" << std::endl;
    return 1;
  }
  if (linesTotal!=45)
  {
    std::cout << "Error: expected line count to be 45, but found \"" << linesTotal
              << "\" instead!" << std::endl;
    return 1;
  }

  libuhs::HintChunk hc(1, "", std::vector<std::string>());
  try
  {
    bool success = hc.readFromStream(inFileStream, linesTotal);
    inFileStream.close();
    if (!success)
    {
      std::cout << "Error: Reading hint chunk failed!" << std::endl;
      return 1;
    }
    std::cout << "Info: Successfully read hint chunk from file!" << std::endl;
    std::cout << "  Label: " << hc.getLabel() << std::endl
              << "  # of hints: " << hc.hints.size() << std::endl;
    /*
    for (auto&& item: hc.hints)
    {
      std::cout << "  Hint: " << item << std::endl;
    } //for
    */
    if (hc == cExpected)
    {
      std::cout << "Hint chunk data matches the expected result." << std::endl;
    }
    else
    {
      std::cout << "Error: Hint chunk data does NOT match the expected result." << std::endl;
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
