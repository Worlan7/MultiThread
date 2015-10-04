/* Lab0.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains the main functions used in Lab 0, which is concerned with
* a multithreaded approach to building a play script from a given
* configuration file. Refer to Readme for more details.
*/
#include "stdafx.h"
#include "Lab0Extra.h"


int main(int argc, char* argv[])
{
	if (argc != MIN_ARGS)
	{
		std::cout << "Usage: " << argv[0]
			<< "<script_fragment_file_name>"
			<< "<configuration_file_name>"
			<< "<'play_name'>"
			<< std::endl;
	}
	else
	{
		std::string script = argv[1];
		std::string configFile = argv[2];
		std::string playName = argv[3];
		std::string playLine;
		std::string currentChar;
		std::set<std::string> characters;
		std::vector<Line> lines;
		bool isCharacter = false;

		std::ifstream scriptFile(script);
		if (scriptFile.is_open())
		{
			//get first character name
			for (int i = 0; std::getline(scriptFile, playLine); i++)
			{
				std::istringstream iss(playLine);
				std::string characterName;
				if ((i == 0 || isCharacter) && !playLine.empty())
				{
					if (iss >> characterName)
					{
						//character names will end with fullstops. We do not want that
						characterName.pop_back();
						currentChar = characterName;
						characters.insert(characterName);
					}
					isCharacter = false;
					if (i != 0)
					{
						//Do not count Character lines as spoken line numbers
						i-=2;
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
				if (partOutput.is_open())
				{
					for (Line l : lines)
					{
						if (l.lineCharacter == character)
						{
							partOutput << l.lineNumber << " " << l.lineText << std::endl;
						}
					}
				}
				else
				{
					std::cout << "Could not create " << characterFile << std::endl;
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
	return 0;
}

