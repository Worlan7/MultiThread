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
