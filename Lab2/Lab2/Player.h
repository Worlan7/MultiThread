/* Player.h
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
*
* Author: Joe Fiala
* E-mail: joeafiala@wustl.edu
*
* This contains declarations for a Player class, used in Lab 1,
* which is concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "Play.h"
#include <sstream>
#include <fstream>
#include <thread>
#include <string>

//Player class models a named character in a play. Each object has its own 
//thread
class Player
{
public:
	Player(Play &play) : play_(play)
	{
		plThread_ = std::thread();
	};

	//Overload default copy constructor, since it would attempt to copy
	//thread member variable otherwise. Should the copy constructor just be
	//=delete?
	Player(const Player & original) : play_(original.play_)
	{
		plThread_ = std::thread();
	};
	//Player methods
	int read(std::string charName, std::string fileName);
	void act();
	void enter();
	void addMessage(Message m);
	void exit();

private:
	std::vector<Line> structuredLines_;
	Play& play_;
	std::thread plThread_;
	//input queue for Active Object player
	PlayerQueue<Message> inputQueue;
};

#endif