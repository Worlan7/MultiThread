/* Player.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
*
* Author: Joe Fiala
* E-mail: joeafiala@wustl.edu
*
* This contains definitions for a Player class used in Lab 1, which is
* concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#include "stdafx.h"
#include "Player.h"

//Simple function that repeatedly reads a line from the file, and inserts valid
//lines into the Player structured_lines container member variable.
int Player::read(std::string fileName)
{
	structuredLines_.clear();

	if (!utility::doesFileExist(fileName))
	{
		return fileNotOpened;
	}

	std::ifstream ifs(fileName);

	std::string line;
	if (ifs.is_open())
	{
		while (std::getline(ifs, line))
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
	ifs.close();
	return runningFine;
}

//Repeatedly passes the iterator into a call to the recite method of the Play 
//class until the iterator is past the last structured_line in the container.
void Player::act(int fragmentNum)
{
	play_.enter(fragmentNum);

	for (auto it = structuredLines_.begin(); it != structuredLines_.end(); it++)
	{
		play_.recite(it, fragmentNum);
	}

	play_.exit();

	//QUESTION: are these needed?
	//currentLine = INT_MAX;	//used to show that current line does not exist.
	//play_.numDone++;		//used to show that player is done.
}

//Launches new thread using move semantics. Calls the read, then act methods.
void Player::enter()
{
	std::unique_lock<std::mutex> lock(pMutex_);

	while (isBusy_)
	{
		pCV_.wait(lock);
	}

	isBusy_ = true;

	std::thread plThread([this]{
		this->read(charFileName_);
		this->act(fragmentNum_);
		this->exit();
	});
	plThread_ = std::move(plThread);

	plThread_.join();

	lock.unlock();
	pCV_.notify_all();
}

//Calls join method iff thread member variable is joinable
void Player::exit()
{
	play_.exit; //are we done with this here?
	isBusy_ = false;
	pCV_.notify_all();
}

void doIt()
{

}