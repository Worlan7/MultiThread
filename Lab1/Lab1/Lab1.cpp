/* Lab1.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains the main functions used in Lab 1, which is concerned with
* a multithreaded approach to building a play script from a given
* configuration file. Refer to Readme for more details.
*/


#include "stdafx.h"
#include "Lab1.h"

// main function reads in a config file, and based on whether or not it's 
// formatted correctly, then constructs a Play object using the name of the 
// play provided, constructs Players for each well defined line in the config
// file, and causes them to enter and exit the play as desired.
int main(int argc, char* argv[])
{
	if (argc < MIN_ARGS)
	{
		std::cout << "usage: " << argv[ZERO] << "<configuration_file_name"
			<< std::endl;
		return notEnoughArgs;
	}
	else
	{
		std::vector<Player> players;
		std::vector<std::string> charNames;
		std::vector<std::ifstream> charFiles;

		bool atLeastOneCharacter = false;
		std::ifstream configFile(argv[ONE]);
		std::string playName;
		std::string configLine;

		if (configFile.is_open())
		{
			//get play/play fragment name
			std::getline(configFile, playName);
			Play mainPlay(playName);
			//Player definitions
			for (int i = 1; std::getline(configFile, configLine); i++)
			{
				std::istringstream iss(configLine);
				std::string charName;
				std::string charFile;
				if ((iss >> charName >> charFile))
				{
					std::ifstream charFileStream(charFile);
					if (charFileStream.is_open())
					{
						charFileStream.close();
						charNames.push_back(charName);
						charFiles.push_back(std::ifstream(charFile));
						atLeastOneCharacter = true;
					}
					else
					{
						std::cout << "File " << charFile << " not opened"
							<< std::endl;
					}
				}
				else
				{
					std::cout << "Skipping line " << i << " in " << argv[ONE]
						<< ", malformed character definition at line"
						<< std::endl;
				}
			}
			if (atLeastOneCharacter)
			{
				//construct players
				for (unsigned i = BASE; i < charNames.size(); i++)
				{
					players.push_back
					(
						std::move
						(
							Player
							(
								std::ref(mainPlay), 
								charNames[i], 
								charFiles[i]
							)
						)
					);
				}
				//After all players have been constructed, call each player's 
				//enter method.
				for (Player& p : players)
				{
					p.enter();
				}
				//Used to ensure that malformed inputs are handled
				while (mainPlay.numDone != players.size())
				{
					int low = INT_MAX;
					for (Player& p : players)
					{
						if (p.currentLine < low)
						{
							low = p.currentLine;
						}
					}		
					if (*mainPlay.counter < low)
					{
						 //This means that there is a gap, so we advance the 
						//counter to the lowest available line
						std::unique_lock<std::mutex> gapLk(*mainPlay.barrier);
						*mainPlay.counter = low;
						gapLk.unlock();
						mainPlay.conVar.notify_all();
					}
				}

				//After enter has been called on all Player objects, call exit
				for (Player& p : players)
				{
					p.exit();
				}
			}
			else
			{
				//Configuration file did not contain any valid character 
				//definition
				std::cout << "No valid character definitions" << std::endl;
				return noValidCharDef;
			}
		}
		else
		{
			std::cout << "File " << argv[ONE] << " not opened" << std::endl;
			return fileNotOpened;
		}
		configFile.close();
	}
	return runningFine;
}

