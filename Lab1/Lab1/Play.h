/* Play.h
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains declarations for a Play class, used in Lab 1, 
* which is concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#ifndef PLAY_H
#define PLAY_H

#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <algorithm>

enum numConstants : int
{
	ZERO = 0,
	ONE = 1,
	TWO = 2,
};

enum programErrors : int
{
	runningFine = 0,
	notEnoughArgs = 1,
	fileNotOpened = 2,
	noValidCharDef = 3,
};


struct Line
{
	//constructors 
	Line(){};
	Line(int num, std::string character, std::string txt)
		: lineNumber(num), lineCharacter(character), lineText(txt) {};
	//for comparison
	bool operator< (const Line &) const;
	//member variables
	int lineNumber;
	std::string lineCharacter;
	std::string lineText;
};

class Play
{
public:
	Play(std::string playName) : playName_(playName), counter_(ONE),
	numDone(ZERO), counter(&counter_), barrier(&barrier_) {};
	void recite(std::vector<Line>::iterator &lineIt);
	int* counter;		//exposed to main thread
	std::mutex* barrier;	//exposed to main thread
	std::condition_variable conVar;		//exposed to main thread
	int numDone;


private:
	std::string playName_;
	std::string speakingCharacter_;
	std::mutex barrier_;
	int counter_;
};

#endif
