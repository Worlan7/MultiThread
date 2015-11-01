/* Lab2.cpp
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* 
* Author: Joe Fiala
* E-mail: joeafiala@wustl.edu
*
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
			<< "<optional: players_to_construct>"
			<< "<optional: -override>"
			<< std::endl;
		return notEnoughArgs;
	}
	else
	{
		unsigned int minPlayers;
		//flag to note if user passed in min number of players to use
		bool minGiven = false;
		bool flag = false;

		if (argc > MIN_ARGS)
		{
			if (argc > TWO) //if the override argument was given
			{
				if (argv[THREE] == "-override")
				{
					flag = true;
				}
			}

			std::istringstream iss(argv[TWO]);
			if (iss >> minPlayers)
			{
				minGiven = true;
			}
			else
			{
				std::cout << "Wrong arg passed in for min number of players"
					<< std::endl;
				//should we exit the program here or just ignore the fact that 
				//a wrong argument was provided for minPlayers?
				return wrongArgs;
			}
		}

		std::string scriptFile = argv[ONE];
		if (minGiven)
		{
			try{
				Director playDirector(scriptFile, flag, minPlayers);
				playDirector.cue();
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else
		{
			try{
				Director playDirector(scriptFile, flag);
				playDirector.cue();
			}
			catch (std::exception& e)
			{
				//TODO
				std::cout << e.what() << std::endl;
			}
		}

		return runningFine;
	}
}
