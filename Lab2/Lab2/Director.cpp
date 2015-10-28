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
					std::shared_ptr<Fragment> newFragment(new Fragment);
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
						mainScript.fragments.push_back(newFragment);
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


	/*
    if (!utility::doesFileExist(nameOfScriptFile))
    {
        throw directorException();
    }
    
    ifstream ifs(nameOfScriptFile);
    string sceneStr;
    const string sceneToken = "[scene]";
    vector<string> titlesOfScenes;
    vector<int> numPartConfigLines;
    numPartConfigLines.push_back(START_POSITION);
    unsigned int bestSum = START_POSITION;
    bool lastLineConfig = false;
	script mainScript;

    //this can be changed to fit the overall design of parsing strings better

	//goes through the main script to find scene fragments
    while (!ifs.eof() && ifs.good())
    {
		shared_ptr<fragment> newFragment(new fragment);

        getline(ifs, sceneStr); //gets next line
        
        if (sceneStr.empty()) continue; //no need to do unneccesary work if the line is blank, so skip
        
        if (sceneStr.size() > sceneToken.size() && sceneToken.compare(sceneStr.substr(START_POSITION, sceneToken.size())) == STRINGS_EQUAL)
        {
            string sceneTitle = sceneStr.substr(sceneToken.size(), sceneStr.size() - sceneToken.size());
            titlesOfScenes.push_back(sceneTitle);
            lastLineConfig = false;
        }
        else
        {
            string configStr = sceneStr; //just for naming/ease of understanding
            
            if (!utility::doesFileExist(configStr)) continue;
            
            ifstream configIfs(configStr); //configuration file
            
            string partDefLine;
            int numPartDefLines = START_POSITION;

			//goes through scene fragment files to find part files
            while (!configIfs.eof() && configIfs.good())
            {
                getline(configIfs, partDefLine);
                
                if (partDefLine.empty()) continue;

				std::stringstream iss(partDefLine);

				std::string characterName;
				std::string partFileName;

				iss >> characterName >> partFileName; //do we need anything here for safety?

				//creating part and adding to fragment
				shared_ptr<part> newPart(new part(characterName, partFileName));
				newFragment->partContainer.push_back(move(newPart));

                numPartDefLines++;
            }
            
            numPartConfigLines.insert(numPartConfigLines.begin(), numPartDefLines);
            int sumVal = numPartConfigLines[FIRST_OF_CONSECUTIVE] + numPartConfigLines[SECOND_OF_CONSECUTIVE];
            if (sumVal > bestSum) bestSum = sumVal;
            
            if (lastLineConfig)
            {
                titlesOfScenes.push_back(""); //push back empty string
            }
            
            lastLineConfig = true;
        }

		//adding fragment to script
		mainScript.fragmentContainer.push_back(move(newFragment));
    }
    
    shared_ptr<Play> temp(new Play(ref(titlesOfScenes)));
    
    playSharedPointer = temp;
    
    int numPlayers = max(playersToConstruct, bestSum);
    
    for (int i = 0; i < numPlayers; i++)
    {
        shared_ptr<Player> player(new Player(playSharedPointer));
        playerContainer.push_back(move(player));
    }
	*/
}


Director::~Director()
{
}

void Director::processScriptFile()
{
	//traverses Script struct for fragments
	for (mainScript.fragmentIter; mainScript.fragmentIter != mainScript.fragments.end(); mainScript.fragmentIter++)
	{
		std::shared_ptr<Fragment> f(std::move(mainScript.fragmentIter->get()));
		//traverses Fragment struct for parts
		for (f->partIter; f->partIter != f->parts.end(); f->partIter++)
		{
			std::shared_ptr<Part> p(std::move(f->partIter->get()));

			//adds part to queues
			partQueue.push(std::move(p));
		}
	}

	//adds the end signal to the queue, since there are no parts remaining in the script
	std::shared_ptr<Part> theEnd(new Part("ENDOFPARTS", ""));
	partQueue.push(std::move(theEnd));
}

void Director::handOffWork()
{
	while (true)
	{
		//if there is work in the queue
		if (!partQueue.empty())
		{
			//this signals the end of the parts from the script
			if (partQueue.front()->characterName.compare("ENDOFPARTS") == ZERO) //presumably we'd just add a part with "ENDOFPARTS" somewhere above
			{
				partQueue.pop();
				return;
			}


			//gets the Player with the min # of tasks left and gives them the front of the queue of Parts
			std::min(playerContainer.begin(), playerContainer.end())->get()->enter(partQueue.front()); 
			//^^^for above: assuming the Player.enter takes some Part parameter
			//maybe we should have a separate Player method that simply adds tasks
			//(rather than lumping that into the enter method)?
			
			
			partQueue.pop();
		}
	}
}

void Director::cue()
{
	std::thread process(&processScriptFile);
	std::thread handOff(&handOffWork);
	process.join();
	handOff.join();
}