/* Play.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains definitions for a Play class used in Lab 1, which is
* concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#include "stdafx.h"
#include "Play.h"

//Legacy function from lab 0. Left in case sorting required.
bool Line::operator<(const Line &rLine) const
{
	return (lineNumber < rLine.lineNumber);
}


//The recite method compares the value of the counter_ member variable with the
//line number of the structured_line pointed to by the iterator. If counter_ is
//less than structured_line's number then this method repeatedly waits on 
//conVar_ until counter_ reaches that number. When reached, this method prints
//out the line to cout, increments the iterator, notifies all other threads
//waiting on conVar_ and returns.
void Play::recite(std::vector<Line>::iterator &lineIt)
{
	std::unique_lock<std::mutex> reciteLk(barrier_);
	//To avoid potential deadlocks, make sure counter_ is not > structured_line
	//number
	if (counter_ > lineIt->lineNumber)
	{
		std::cerr << "Badly formed script fragment provided." <<
			" Counter > line num" << std::endl;
		lineIt++;
		reciteLk.unlock();
		conVar.notify_all();
		return;
	}

	//wait until counter_ == lineNumber
	conVar.wait(reciteLk, [=]{return counter_ == lineIt->lineNumber; });
	
	//Keep track of speaking character, and output specified format when 
	//speaking character changes
	if (lineIt->lineCharacter != speakingCharacter_)
	{
		speakingCharacter_ = lineIt->lineCharacter;
		std::cout << std::endl << speakingCharacter_ << "." << std::endl;
	}
	std::cout << lineIt->lineText << std::endl;
	lineIt++, counter_++;
	reciteLk.unlock();
	conVar.notify_all();
}




