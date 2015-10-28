#include "stdafx.h"
#include "Utility.h"


bool utility::doesFileExist(const std::string& file)
{
	std::ifstream ifs(file);
	if (ifs.good()) //exists and is readable
	{
		ifs.close();
		return true;
	}
	else //problem with file
	{
		ifs.close();
		return false;
	}
}

//Trims whitespace from string's beginning & end (see http://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string )
std::string utility::trim(const std::string &str, const std::string &whitespace = " \t")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return "";

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + ONE;

	return str.substr(strBegin, strRange);
}

part::part(std::string name, std::string file)
{
	characterName = name;
	fileName = file;

	if (!utility::doesFileExist(file))
	{
		//problem with retrieving lines
	}
	else
	{
		// @Elom -- feel free to revise/redo this as you see fit
		// this is essentially just what I did for Lab 1 to parse
		// a string into the line # and the line text
		// if you have a more elegant solution, feel free to 
		// add that here instead
		std::ifstream ifs(fileName);
		std::string line;
		std::string deli = " "; //used for whitespace delimeter

		while (!ifs.eof() && ifs.good())
		{
			getline(ifs, line);

			if (line.find_first_of(deli) == std::string::npos) continue;

			//get line number
			std::string num = line.substr(ZERO, line.find(deli));
			int number = atoi(num.c_str());
			
			if (number == ZERO) continue; //failed conversion

			//get text
			line.erase(ZERO, line.find(deli) + deli.length());
			std::string text = utility::trim(line);

			if (text.empty()) continue;

			//creating the structured line
			Line tempLine(number, characterName, text);
			linesOfPart.push_back(tempLine);
		}

		//setting the iterator to the beginning of the container
		linesOfPartIterator = linesOfPart.begin();
	}
}
