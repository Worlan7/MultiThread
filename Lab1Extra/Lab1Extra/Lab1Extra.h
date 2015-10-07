/* Lab1Extra.h
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains the necessary declarations for functions
* used in Lab 1 Extra.
* Refer to Readme for more details.
*/

#ifndef LAB_1_EXTRA_H
#define LAB_1_EXTRA_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>
#include <cstdlib>

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
	wrongArgs = 4,
};

//command args
int PROGRAM = 0;
int SCRIPT = 1;
const int OPT = 1;
int CONFIG_FILE = 2;
int PLAY_NAME = 3;
//misc
const int BASE = 0;
const time_t SEED = 0;
const int SKIP = 2;
const int MIN_ARGS = 4;

const std::string SCRAMBLE = "SCRAMBLE";

#endif