/* Play.h
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains declarations for a Play class, used in Lab 0, 
* which is concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#ifndef PLAY_H
#define PLAY_H

#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <algorithm>


struct Line{
	//constructors 
	Line(){};
	Line(int num, std::string character, std::string txt)
		: lineNumber(num), lineCharacter(character), lineText(txt) {};
	bool operator< (const Line &) const;
	//member variables
	int lineNumber;
	std::string lineCharacter;
	std::string lineText;
};

class Play{
private:
	std::string playName_;
	std::vector<Line> structuredLines_;
	std::mutex barrier_;

public:
	Play(std::string playName) : playName_(playName) {};
	Play &operator<< (Line line);
	void print(std::ostream &out);
};

#endif
