/* Play.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains definitions for a Play class used in Lab 0, w0hich is
* concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#include "stdafx.h"
#include "Play.h"


bool Line::operator<(const Line &rLine) const
{
	return (lineNumber < rLine.lineNumber);
}


Play &Play::operator<< (Line line)
{
	std::lock_guard<std::mutex> lock_play(barrier_);
	structuredLines_.push_back(line);
	return *this;
}


void Play::print(std::ostream &out)
{
	std::sort(structuredLines_.begin(), structuredLines_.end());
	std::string speakingCharacter = structuredLines_.front().lineCharacter;
	out << speakingCharacter << std::endl;
	for (Line &line : structuredLines_)
	{
		if (line.lineCharacter != speakingCharacter)
		{
			speakingCharacter = line.lineCharacter;
			out << std::endl << speakingCharacter << "." << std::endl;
		}
		out << line.lineText << std::endl;
	}
}
