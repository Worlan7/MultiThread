/* Lab1.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains the main functions used in Lab 0, which is concerned with
* a multithreaded approach to building a play script from a given
* configuration file. Refer to Readme for more details.
*/


#include "stdafx.h"
#include "Lab1.h"

//Simple function run by threads that takes a reference to a non-const Play
//object, a reference to a const C++ style string (character's name), and a 
//reference to a non-const ifs object (for a character part file).
//It repeatedly reads a line from the file, and if the line is a valid
//structured line, it inserts the structured line into the Play object.
void readLines(Play &playObject, const std::string &charName,
	std::ifstream &charFile)
{
	std::string line;
	std::vector<Line> structuredLines;

	while (std::getline(charFile, line))
	{
		std::istringstream iss(line);
		int lineNum;
		std::string lineText;
		//consumes line number, and any whitespace afterwards, then inserts 
		//the remainder of the line into lineText.
		if ((iss >> lineNum >> std::ws) && std::getline(iss, lineText))
		{
			Line structuredLine(lineNum, charName, lineText);
			playObject << structuredLine;
		}
	}
	charFile.close();
}

//Main function reads in a config file and spawns a number of threads to
//read lines from play described by config file as per read me, and build
//a play object.
int main(int argc, char* argv[])
{
	if (argc < MIN_ARGS)
	{
		std::cout << "usage: " << argv[0] << "<configuration_file_name"
			<< std::endl;
		return notEnoughArgs;
	}
	else
	{
		std::vector<std::thread> characterThreads;
		bool atLeastOneCharacter = false;
		std::ifstream configFile(argv[1]);
		std::string playName;
		std::string configLine;

		if (configFile.is_open())
		{
			//get play/play fragment name
			std::getline(configFile, playName);
			Play mainPlay(playName);

			//definition of character's parts to be run in separate threads
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
						characterThreads.push_back(
							std::thread(readLines,
							std::ref(mainPlay),
							charName,
							std::ifstream(charFile)));
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
					std::cout << "Skipping line " << i << " in " << argv[1]
						<< ", malformed character definition at line"
						<< std::endl;
				}
			}
			if (atLeastOneCharacter){
				//Join the threads with the main thread
				for (std::thread &charThread : characterThreads)
				{
					charThread.join();
				}
				mainPlay.print(std::cout);
			}
			else
			{
				//Configuration file did not contain any valid character 
				//definition
				std::cout << "No valid character definition lines" << std::endl;
				return noValidCharDef;
			}
		}
		else
		{
			std::cout << "File " << argv[1] << " not opened" << std::endl;
			return fileNotOpened;
		}
		configFile.close();
	}
	return runningFine;
}

