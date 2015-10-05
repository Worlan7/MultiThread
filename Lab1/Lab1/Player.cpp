/* Player.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains definitions for a Player class used in Lab 1, which is
* concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#include "stdafx.h"
#include "Player.h"

//Simple function that repeatedly reads a line from the file, and inserts valid
//lines into the Player structured_lines container member variable.
int Player::read()
{
	std::cout << "This is " << charName_ << std::endl;
	std::string line;
	if (plFile_.is_open())
	{
		while (std::getline(plFile_, line))
		{
			std::istringstream iss(line);
			int lineNum;
			std::string lineText;
			//consumes line number, and any whitespace afterwards, then inserts 
			//the remainder of the line into lineText.
			if ((iss >> lineNum >> std::ws) && std::getline(iss, lineText))
			{
				Line structuredLine(lineNum, charName_, lineText);
				structuredLines_.push_back(structuredLine);
			}
		}
	}
	else
	{
		return fileNotOpened;
	}
	//reorder container by line number
	std::sort(structuredLines_.begin(), structuredLines_.end());
	plFile_.close();
	return runningFine;
}


void Player::act()
{
	std::vector<Line>::iterator lineIt = structuredLines_.begin();
	while (lineIt != structuredLines_.end())
	{
		play_.recite(lineIt);
	}
}


void Player::enter()
{
	std::thread plThread([this]{
		this->read();
		this->act();
	});
	plThread_ = std::move(plThread);
}


void Player::exit()
{
	if (plThread_.joinable())
	{
		plThread_.join();
	}
}