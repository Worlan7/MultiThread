#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include "Play.h" //for Line...maybe move that here?

enum numConstants : int
{
	ZERO = 0,
	ONE = 1,
	TWO = 2,
};

namespace utility
{
	static bool doesFileExist(const std::string& file);
	static std::string trim(const std::string &str, const std::string &whitespace);
};

struct part
{
	part(std::string name, std::string file);
	std::string characterName;
	std::string fileName;
	std::vector<Line> linesOfPart;
	std::vector<Line>::iterator linesOfPartIterator;
};

struct fragment
{
	std::vector<shared_ptr<part>> partContainer;
	std::vector<shared_ptr<part>>::iterator partContinerIterator;
};

struct script
{
	std::vector<shared_ptr<fragment>> fragmentContainer;
	std::vector<shared_ptr<fragment>>::iterator fragmentContinerIterator;
};