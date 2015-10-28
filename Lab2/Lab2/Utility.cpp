#include "stdafx.h"
#include "Utility.h"


Part::Part(std::string name, std::string file) :characterName(name),
	fileName(file)
{
	std::ifstream partFile(fileName);
	if (partFile.is_open())
	{
		std::string line;
		while (std::getline(partFile, line))
		{
			std::istringstream iss(line);
			int lineNum;
			std::string lineText;
			//consumes line number, and any whitespace afterwards, then inserts 
			//the remainder of the line into lineText.
			if ((iss >> lineNum >> std::ws) && std::getline(iss, lineText))
			{
				Line structuredLine(lineNum, characterName, lineText);
				partLines.push_back(structuredLine);
			}
		}
	}
	else
	{
		//problem with retrieving lines. Throw exception??

	}
	//setting the iterator to the beginning of the container
	partLinesIter = partLines.begin();
}
