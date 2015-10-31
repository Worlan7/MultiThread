/* Play.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
*
* Author: Joe Fiala
* E-mail: joeafiala@wustl.edu
*
* This contains definitions for a Play class used in Lab 2, which is
* concerned with a multithreaded approach to building a play 
* from a given script file. Refer to Readme for more details.
*/

#include "stdafx.h"
#include "Play.h"

//Legacy function from lab 0. Left in case sorting required.
bool Line::operator<(const Line &rLine) const
{
	return (lineNumber < rLine.lineNumber);
}


//The recite method compares the value of the counter_ member variable with the
//line number of the structured_line pointed to by the iterator. It also 
//compares the sceneFragmentCounter_ with the curSceneFragment. While
//sceneFragmentCounter_ is less than the passed scene fragment number, or is 
//equal but lineCounter_ < line number in the structured line referenced by 
//the passed iterator, the recite method repeatedly waits on conVar, until 
//lineCounter and sceneFragmentCounter_ reach the values given in the the 
//corresponding passed data. When reached, this method prints
//out the line to cout, increments the iterator, notifies all other threads
//waiting on conVar_ and returns.
void Play::recite(std::vector<Line>::iterator &lineIt, int curSceneFragment)
{
	std::unique_lock<std::mutex> reciteLk(barrier_);
	//To avoid potential deadlocks, make sure neither sceneFragmentCounter_ is
	// > curSceneFragment nor lineCounter_ is > structured_line number
	if (sceneFragmentCounter_ >= curSceneFragment) 
	{
		if (sceneFragmentCounter_ > curSceneFragment || 
			lineCounter_ > lineIt->lineNumber )
		{
			std::cerr << "Badly formed script fragment provided. " <<
				"lineCounter > line num or sceneFragmentCounter > currentScene" 
				<< std::endl;
			lineIt++;
			reciteLk.unlock();
			conVar.notify_all();
			return;
		}
	}

	//wait until we are in current scene and lineCounter_ == lineNumber
	conVar.wait(reciteLk, [=]{return (lineCounter_ == lineIt->lineNumber)
		&& (sceneFragmentCounter_ == curSceneFragment); });
	
	//Keep track of speaking character, and output specified format when 
	//speaking character changes
	if (lineIt->lineCharacter != speakingCharacter_)
	{
		speakingCharacter_ = lineIt->lineCharacter;
		std::cout << std::endl << speakingCharacter_ << "." << std::endl;
	}
	std::cout << lineIt->lineText << std::endl;
	lineIt++, lineCounter_++;
	reciteLk.unlock();
	conVar.notify_all();
}

void Play::enter(int sceneFragment)
{

	std::unique_lock<std::mutex> enterLk(barrier_);
	if (sceneFragment < sceneFragmentCounter_)
	{
		//throw an exception
		throw invalidSceneEnterException();
		return;
	}
	else if (sceneFragment > sceneFragmentCounter_)
	{
		//wait here till sceneFragment == sceneFragmentCounter_
		conVar.wait(enterLk, [=]{
			return sceneFragment == sceneFragmentCounter_;
		});
	}
	//Guaranteed that sceneFragment == sceneFragmentCounter_ at this stage
	onStage_++;
	enterLk.unlock();
	conVar.notify_all();
	return;
}

void Play::exit()
{
	std::unique_lock<std::mutex> exitLk(barrier_);
		
	if (onStage_ < 1){
		throw invalidOnStageException();
	}
	else if (onStage_ > 1)
	{
		onStage_--;
		exitLk.unlock();
		conVar.notify_all();
	}
	else
	{
		onStage_--;
		sceneFragmentCounter_++;
		if (sceneIt_ != sceneNames_.end())
		{
			if (!sceneIt_->empty())
			{
				std::cout << *sceneIt_ << std::endl;
				sceneIt_++;
				exitLk.unlock();
				conVar.notify_all();
			}
		}
	}
}



