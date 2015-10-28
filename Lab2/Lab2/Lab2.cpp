/* Lab1.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains the main functions used in Lab 2, which is concerned with
* a multithreaded approach to building a play script from a given
* configuration file. Refer to Readme for more details.
*/


#include "stdafx.h"
#include "Lab2.h"

// main function reads in a config file, and based on whether or not it's 
// formatted correctly, then constructs a Play object using the name of the 
// play provided, constructs Players for each well defined line in the config
// file, and causes them to enter and exit the play as desired.
int main(int argc, char* argv[])
{
	if (argc < MIN_ARGS)
	{
		std::cout << "usage: " << argv[ZERO] << "<script_file_name>"
			<< std::endl;
		return notEnoughArgs;
	}
	else
	{
		unsigned int minPlayers;
		//flag to note if user passed in min number of players to use
		bool minGiven = false;		

		if (argc > MIN_ARGS)
		{
			std::istringstream iss(argv[TWO]);
			if (iss >> minPlayers)
			{
				minGiven = true;
			}
			else
			{
				std::cout << "Wrong arg passed in for min number of players"
					<< std::endl;
				return wrongArgs;
			}
		}

		Director playDirector;
		if (minGiven)
		{

		}
		else
		{
		}
	
	return runningFine;
}

