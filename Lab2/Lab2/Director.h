#pragma once
#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <queue>
#include "Play.h"
#include "Player.h"


class Director
{
public:
    Director(std::string scriptFile, unsigned int minPlayers = ZERO);
    ~Director();
	std::vector<std::string> sceneTitles;
    void cue();
private:
    std::shared_ptr<Play> playSharedPointer;
    std::vector<std::shared_ptr<Player>> playerContainer;
	std::queue<std::shared_ptr<Part>> partQueue;
	Script mainScript;
	void processScriptFile();
};

#endif /* defined DIRECTOR_H */