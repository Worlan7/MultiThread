#pragma once

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

enum numConstants : int
{
	ZERO = 0,
	ONE = 1,
	TWO = 2,
};

enum programErrors : int
{
	runningFine = 0,
	notEnoughArgs = 1,
	fileNotOpened = 2,
	noValidCharDef = 3,
	wrongArgs = 4,
};

struct Line
{
	//constructors 
	Line(){};
	Line(int num, std::string character, std::string txt)
		: lineNumber(num), lineCharacter(character), lineText(txt) {};
	//for comparison
	bool operator< (const Line &) const;
	//member variables
	int lineNumber;
	std::string lineCharacter;
	std::string lineText;
};

struct Part
{
	Part(std::string name, std::string file);
	//member variables
	std::string characterName;
	std::string fileName;
	std::vector<Line> partLines;
	std::vector<Line>::iterator partLinesIter;
};

struct Fragment
{
	std::vector<std::shared_ptr<Part>> parts;
	std::vector<std::shared_ptr<Part>>::iterator partIter = parts.begin();
};

struct Script
{
	std::vector<std::shared_ptr<Fragment>> fragments;
	std::vector<std::shared_ptr<Fragment>>::iterator fragmentIter = 
		fragments.begin();
};

class invalidOnStageException : public std::exception{
public:
	const char * what() const throw()
	{
		return "Invalid Players on Stage Exception\n";
	}
};

class invalidSceneEnterException : public std::exception{
public:
	const char * what() const throw()
	{
		return "Invalid Scene Entry Fragment Exception\n";
	}
};

class directorException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Exception! Script file could not be opened.";
	}
};


