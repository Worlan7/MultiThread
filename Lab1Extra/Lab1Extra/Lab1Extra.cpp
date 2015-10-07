/* Lab1.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains the main functions used in Lab 1extra, which is concerned with
* generating a configuration file describing a play in the format specified
* in the instructions.
*/
#include "stdafx.h"
#include "Lab1Extra.h"


void usage(char* arg_0)
{
	std::cout << "Usage: " << arg_0
		<< "<SCRAMBLE> optional"
		<< "<script_fragment_file_name>"
		<< "<configuration_file_name>"
		<< "<'play_name'>"
		<< std::endl;
}

int main(int argc, char* argv[])
{
	if (argc < MIN_ARGS)
	{
		usage(argv[PROGRAM]);
		return notEnoughArgs;
	}
	else
	{
		std::string script, configFile, playName;
		bool scramble = false;
		if (argv[OPT] != SCRAMBLE)
		{ 
			script = argv[SCRIPT];
			configFile = argv[CONFIG_FILE];
			for (int i = PLAY_NAME; i < argc; i++)
			{
				playName += std::string(argv[i]) + " ";
			}
		}
		else
		{
			scramble = true;
			script = argv[++SCRIPT];
			configFile = argv[++CONFIG_FILE];
			for (int i = ++PLAY_NAME; i < argc; i++)
			{
				playName += std::string(argv[i]) + " ";
			}
		}
		std::string playLine;
		std::string currentChar;
		std::set<std::string> characters;
		std::vector<Line> lines;
		bool isCharacter = false;

		std::ifstream scriptFile(script);
		if (scriptFile.is_open())
		{
			//get first character name
			for (int i = BASE; std::getline(scriptFile, playLine); i++)
			{
				std::istringstream iss(playLine);
				std::string characterName;
				if ((i == BASE || isCharacter) && !playLine.empty())
				{
					if (iss >> characterName)
					{
						//character names will end with fullstops. 
						//We do not want that
						characterName.pop_back();
						currentChar = characterName;
						characters.insert(characterName);
					}
					isCharacter = false;
					if (i != BASE)
					{
						//Do not count Character lines as spoken line numbers
						i-=SKIP;
					}
				}
				else
				{
					if (playLine.empty())
					{
						//Means next line is a character line
						isCharacter = true;
					}
					else
					{
						Line struct_line(i, currentChar, playLine);
						lines.push_back(struct_line);
					}
				}
			}

			//create config file
			std::ofstream configOutput(configFile);
			if (configOutput.is_open())
			{
				configOutput << playName << std::endl;
				for (std::string chara : characters)
				{
					configOutput << chara << " " << 
						chara << ".txt" << std::endl;
				}
			}
			else
			{
				std::cout << "Could not create " << configFile << std::endl;
				return couldNotCreate;
			}
			configOutput.close();
			//creating the part files
			for (std::string character : characters)
			{
				std::string characterFile = character + ".txt";
				std::ofstream partOutput(characterFile);
				if (scramble)
				{
					std::srand(unsigned(std::time(SEED)));
					std::random_shuffle(lines.begin(), lines.end());
				}
				if (partOutput.is_open())
				{
					for (Line l : lines)
					{
						if (l.lineCharacter == character)
						{
							partOutput << l.lineNumber << " " 
								<< l.lineText << std::endl;
						}
					}
				}
				else
				{
					std::cout << "Could not create " << characterFile 
						<< std::endl;
					return couldNotCreate;
				}
				partOutput.close();
			}
			
		}
		else
		{
			std::cout << "File " << script << " could not be opened" 
				<<std::endl;
			return fileNotOpened;
		}
	}
	return runningFine;
}

