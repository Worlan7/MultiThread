/* Lab1.h
* Author: Elom Kwame Worlanyo
* E-mail: elomworlanyo@wustl.edu
* This contains the necessary declarations for functions
* used in Lab 0, which is concerned witha multithreaded approach to
* building a play script from a given configuration file.
* Refer to Readme for more details.
*/

#ifndef LAB_1_H
#define LAB_1_H

#include "Play.h"
#include <sstream>
#include <fstream>
#include <thread>

enum programErrors : int
{
	runningFine = 0,
	notEnoughArgs = 1,
	fileNotOpened = 2,
	noValidCharDef = 3,
};

const int MIN_ARGS = 2;
#endif