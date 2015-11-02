#include "stdafx.h"
#include "Director.h"
#include "Utility.h"


Director::Director(std::string scriptFile, bool flag, unsigned int minPlayers)
{
	allSignalled_ = false;
	std::ifstream scriptFileStream(scriptFile);
	std::string scriptLine;
	const std::string scenePrefix = "[scene] ";
	//used to keep track of the number of part config lines in each scene
	//fragment config file  
	std::vector<int> numPartConfigLines;
	//maximum number of part lines in two consecutive script fragments
	unsigned int maxConsecPartLines = 0;
	bool lastLineConfig = false;

	if (scriptFileStream.is_open())
	{
		//Iterating line by line through the script file stream.
		while (std::getline(scriptFileStream, scriptLine))
		{
			if (scriptLine.empty())
			{
				continue;
			}

			std::istringstream iss(scriptLine);
			//Test if current script line is prefixed by "[scene] "
			if (!scriptLine.compare(ZERO, scenePrefix.size(), scenePrefix))
			{
				//Then "[scene] " is a prefix and we extract the scene title
				std::string sceneTitle = scriptLine.substr(scenePrefix.size());
				sceneTitles.push_back(sceneTitle);
				//set lastLineConfig flag to false, since this is not a config
				//file line
				lastLineConfig = false;
			}
			else
			{
				//Then "[scene] " is not a prefix and line is config file fpr
				//screen fragments
				std::ifstream fragFile(scriptLine);
				if (fragFile.is_open())
				{
					//TODO: What do we do if fragment contains no valid part 
					//definition lines?
					std::shared_ptr<Fragment> newFragment(new Fragment());
					std::string partDefLine;
					int numPartDefinitions = 0;

					for (int i = 1; std::getline(fragFile, partDefLine); i++)
					{
						if (!partDefLine.empty())
						{
							std::stringstream iss(partDefLine);
							std::string characterName;
							std::string partFileName;

							if (iss >> characterName >> partFileName)
							{
								//valid part definition line, so adding to num
								//of part definition lines
								numPartDefinitions++;
								//creating part and adding to fragment
								std::shared_ptr<Part> newPart(
									new Part(characterName, partFileName)
								);
								//TODO: What if part is not created?
								newFragment->parts.push_back(
									move(newPart)
								);
							}
							else
							{
								std::cout << "Skipping line " << i << " in " 
									<< scriptLine << ", malformed character "
									<< "definition at line" << std::endl;
							}
						}
					}
					//currently only adding scene fragment to script if it
					//contains parts
					if (!newFragment->parts.empty())
					{
						directorScript_.fragments.push_back(newFragment);
						//Storing the number of part definitions
						numPartConfigLines.push_back(numPartDefinitions);
						//if two consecutive config lines, push back empty 
						//string
						if (lastLineConfig)
						{
							std::string empty = " ";
							sceneTitles.push_back(empty);
						}
						//set lastLineConfig flag to true, since this is a 
						//config file line
						lastLineConfig = true;
					}
				}
			}
		}

		//maximum sum of the numbers of part configuration lines that appear 
		//in any two consecutive configuration files
		for (unsigned int i = 0; i < numPartConfigLines.size() - 1; i++)
		{
			unsigned int sum = numPartConfigLines[i] + 
				numPartConfigLines[i + ONE];

			if (maxConsecPartLines < sum)
			{
				maxConsecPartLines = sum;
			}
		}

		playSharedPointer_ = std::make_shared<Play>(std::ref(sceneTitles));
		int numPlayers;

		if (flag)
		{
			numPlayers = minPlayers; //used for override function
		}
		else
		{
			numPlayers = std::max(minPlayers, maxConsecPartLines);
		}
		for (int i = 0; i < numPlayers; i++)
		{
			std::shared_ptr<Player> player(new Player(*playSharedPointer_));
			playerContainer_.push_back(std::move(player));
		}
	}
	else
	{
		//throw exception
		throw directorException();
	}
}

void Director::cue()
{
	//thread that traverses the script data structure, and as it reaches each 
	//scene fragment, takes each part definition line, recruits an available 
	//Player from its container, and calls the Player's enter method to hand 
	//off the information
	std::thread cueThread([this]{
		this->traverseScript();
		this->signalPlayers();
	});

	//waiting for all players to be signalled, then waiting for all players to
	//complete before joining.
	std::unique_lock<std::mutex>lk(mut_);
	directorCond_.wait(lk, [this] {return allSignalled_; });
	//now waiting for all players to complete.
	if (!playerContainer_.empty())
	{
		while (anyActivePlayers())
		{
			std::unique_lock<std::mutex> gapLk(*(playSharedPointer_->barrier));

			int low = INT_MAX;
			for (auto p : playerContainer_)
			{
				if ((p->curScene == *playSharedPointer_->sceneFragmentCounter)
					&& p->isActive)
				{
					if (p->currentLine < low)
					{
						low = p->currentLine;
					}
				}
			}
		
			if (*(playSharedPointer_->lineCounter) < low)
			{
				//This means that there is a gap, so we advance the 
				//counter to the lowest available line
				*(playSharedPointer_->lineCounter) = low;
				gapLk.unlock();
				playSharedPointer_->conVar.notify_all();
			}
		}

		for (auto player : playerContainer_)
		{
			player->exit();
		}
	}

	if (cueThread.joinable())
	{
		cueThread.join();
	}
}

//After this method runs, the director should have a list of messages stored in
//its directorMessages member variable that more or less describe/coordinate
//how the play should proceed.
void Director::traverseScript()
{
	//number of currentSceneFragment
	int sceneFragmentNum = ONE;
	//Traversing through script's scene fragments
	for (auto fragment : directorScript_.fragments)
	{
		for (auto part : fragment->parts)
		{
			Message playerMessage(false, *part, sceneFragmentNum);
			directorMessages_.push(playerMessage);
		}
		sceneFragmentNum++;
	}
	//Passing in special ACT to signify end of play for all players
	for (auto player : playerContainer_)
	{
		Message terminate(true);
		directorMessages_.push(terminate);
	}
}


//Since we are guaranteed that the number of available players will always be 
//greater than or equal to the maximum number of needed players in two 
//consecutive scenes, we can pass off work to all players in a round-robin
//manner, to ensure even distribution of work to each player's work queue. We
//are also assured that no player ever has to represent more than one character
//within the same fragment.
void Director::signalPlayers()
{
	for (int i = 0; !directorMessages_.empty(); i++)
	{
		playerContainer_[i % playerContainer_.size()]->addMessage
			(
				directorMessages_.front()
			);
		directorMessages_.pop();
	}

	for (auto player : playerContainer_)
	{
		player->enter();
	}
	std::unique_lock<std::mutex>lk(mut_);
	allSignalled_ = true;
	lk.unlock();
	directorCond_.notify_one();
}


bool Director::anyActivePlayers()
{
	for (auto player : playerContainer_)
	{
		if (player->isActive)
		{
			return true;
		}
	}
	return false;
}