/* Player.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
*
* Author: Joe Fiala
* E-mail: joeafiala@wustl.edu
*
* This contains definitions for a Player class used in Lab 2, which is
* concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#include "stdafx.h"
#include "Player.h"


void Player::addMessage(Message m)
{
	//Pass in the message given by the Director into the thread safe input
	//queue 
	inputQueue.push(m);
}


//Simple function that repeatedly reads a line from the file, and inserts valid
//lines into the Player structured_lines container member variable.
int Player::read(std::string charName, std::string fileName)
{
	structuredLines_.clear();

	std::ifstream partFile(fileName);

	std::string line;
	if (partFile.is_open())
	{
		while (std::getline(partFile, line))
		{
			std::istringstream iss(line);
			int lineNum;
			std::string lineText;
			//consumes line number, and any whitespace afterwards, then inserts 
			//the remainder of the line into lineText.
			if ((iss >> lineNum >> std::ws) && std::getline(iss, lineText))
			{
				Line structuredLine(lineNum, charName, lineText);
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
	partFile.close();
	return runningFine;
}

//Repeatedly passes the iterator into a call to the recite method of the Play 
//class until the iterator is past the last structured_line in the container.
void Player::act()
{
	while (true)
	{
		std::shared_ptr<Message> activeMessage = inputQueue.waitAndPop();
		if (!activeMessage->endOfPlay)
		{
			std::string charName = activeMessage->inputPart.characterName;
			std::string fileName = activeMessage->inputPart.fileName;
			curScene = activeMessage->sceneFragmentNum;
			if (read(charName, fileName) != runningFine)
			{
				//throw exception
				throw readException();
			}
			else{
				play_.enter(activeMessage->sceneFragmentNum);
				std::vector<Line>::iterator lineIt = structuredLines_.begin();
				while (lineIt != structuredLines_.end())
				{
					currentLine = lineIt->lineNumber;
					play_.recite(lineIt, activeMessage->sceneFragmentNum);
				}
				//character being played by player leaves scene
				play_.exit();
			}
		}
		else
		{
			//end of play, so player bows out entirely.
			isActive = false;
			return;
		}
	}

}

//Launches new thread using move semantics. Calls the act method.
void Player::enter()
{
	std::thread plThread([this]{
		this->act();
	});
	plThread_ = std::move(plThread);

}

//Calls join method iff thread member variable is joinable
void Player::exit()
{
	try{
		if (plThread_.joinable())
		{
			plThread_.join();
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
