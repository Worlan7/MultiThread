/* Lab0Extra.h
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains the necessary declarations for functions
* used in Lab 0 Extra.
* Refer to Readme for more details.
*/

#ifndef LAB_0_EXTRA_H
#define LAB_0_EXTRA_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <thread>

struct Line{
	//constructors 
	Line(){};
	Line(int num, std::string character, std::string txt)
		: lineNumber(num), lineCharacter(character), lineText(txt) {};
	//member variables
	int lineNumber;
	std::string lineCharacter;
	std::string lineText;
};


enum programErrors : int
{
	runningFine = 0,
	notEnoughArgs = 1,
	fileNotOpened = 2,
	couldNotCreate = 3,
};

const int MIN_ARGS = 4;
#endif