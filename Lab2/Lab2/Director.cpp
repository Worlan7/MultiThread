#include "stdafx.h"
#include "Director.h"
#include "Utility.h"


Director::Director(std::string scriptFile, unsigned int minPlayers = 0)
{
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
						directorScript.fragments.push_back(newFragment);
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
		for (int i = 0; i < numPartConfigLines.size() - 1; i++)
		{
			int sum = numPartConfigLines[i] + numPartConfigLines[i + ONE];
			if (maxConsecPartLines < sum)
			{
				maxConsecPartLines = sum;
			}
		}

		std::shared_ptr<Play> temp(new Play(std::ref(sceneTitles)));
		playSharedPointer = temp;
		int numPlayers = std::max(minPlayers, maxConsecPartLines);

		for (int i = 0; i < numPlayers; i++)
		{
			std::shared_ptr<Player> player(new Player(*playSharedPointer));
			playerContainer.push_back(std::move(player));
		}
	}
	else
	{
		//throw exception
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
	//Traversing through script's scene fragments
	while (directorScript.fragmentIter != directorScript.fragments.end())
	{
		//Does it make sense to relinquish the Script struct's ownership of
		//the shared_ptr here instead of simply making a copy?
		std::shared_ptr<Fragment> fragment(directorScript.fragmentIter->get());

		//Traversing through scene fragment's parts
		while (fragment->partIter != fragment->parts.end())
		{
			std::shared_ptr<Part> part(fragment->partIter->get());
			//Need to test whether passing by val or by reference is ok. 
			//Part struct doesn't hold a lot, so copying overhead should be
			//negligible
			Message playerMessage(false, *part.get());
			directorMessages.push(playerMessage);
			
			fragment->partIter++;
		}

		//could add an end of scene fragment token here for testing if needed.
		directorScript.fragmentIter++; 
	}

	//Passing in special ACT to signify end of play for all players
	for (auto player : playerContainer)
	{
		Message terminate(true);
		directorMessages.push(terminate);
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
	for (int i = 0; !directorMessages.empty(); i++)
	{
		playerContainer[i % playerContainer.size()]->enter
			(
				directorMessages.front()
			);
		directorMessages.pop();
	}

}


//What needs to be done here? Could we let a default destructor be created?
Director::~Director()
{
}

