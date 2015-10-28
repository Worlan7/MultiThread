/* Play.h
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains declarations for a Play class, used in Lab 2, 
* which is concerned with a multithreaded approach to building a play script
* from a given configuration file. Refer to Readme for more details.
*/

#ifndef PLAY_H
#define PLAY_H

#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <exception>
#include <condition_variable>
#include <algorithm>
#include "Utility.h"


class Play
{
public:
	Play(std::vector<std::string>& sceneNames) : sceneNames_(sceneNames), 
		lineCounter_(ONE), numDone(ZERO), lineCounter(&lineCounter_), 
		barrier(&barrier_), sceneFragmentCounter_(ONE), onStage_(ZERO)
	{
		sceneIt_ = sceneNames_.begin();
		if (!sceneNames_.empty())
		{
			std::cout << *sceneIt_ << std::endl;
			sceneIt_++;
		}
	};
	void recite(std::vector<Line>::iterator &lineIt, int curSceneFragment);
	void enter(int sceneFragment);
	void exit();
	int* lineCounter;		//exposed to main thread
	std::mutex* barrier;	//exposed to main thread
	std::condition_variable conVar;		//exposed to main thread
	int numDone;


private:
	std::vector<std::string>& sceneNames_;
	std::vector<std::string>::iterator sceneIt_;
	std::string speakingCharacter_;
	std::mutex barrier_;
	int lineCounter_;
	int sceneFragmentCounter_;
	int onStage_;
};

#endif
