/* Player.h
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains declarations for a Player class, used in Lab 2,
* which is concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "Play.h"
#include <sstream>
#include <fstream>
#include <thread>

//Player class models a named character in a play. Each object has its own 
//thread
class Player
{
public:
	Player(Play &play, const std::string &charName, std::ifstream &plFile) :
		play_(play),
		charName_(charName),
		plFile_(plFile)
	{
		plThread_ = std::thread();
	};
	
	//Overload default copy constructor, since it would attempt to copy
	//thread member variable otherwise.
	Player(const Player & original) :
		play_(original.play_),
		charName_(original.charName_),
		plFile_(original.plFile_)
	{
		plThread_  = std::thread();
	};
	//Player methods
	int read();
	void act();
	void enter();
	void exit();
	//mem vars
	int currentLine;

private:
	std::vector<Line> structuredLines_;
	Play& play_;
	std::ifstream& plFile_;
	std::thread plThread_;
	const std::string& charName_;

};

#endif