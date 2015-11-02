#pragma once
#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <memory>
#include "Play.h"
#include "Player.h"


class Director
{
public:
    Director(std::string scriptFile, unsigned int minPlayers = ZERO);
    //~Director();
	std::vector<std::string> sceneTitles;
    void cue();

private:
    std::shared_ptr<Play> playSharedPointer_;
    std::vector<std::shared_ptr<Player>> playerContainer_;
	//Doesn't need to be thread safe, since only a single thread uses it.
	std::queue<Message> directorMessages_;
	Script directorScript_;
	void traverseScript();
	void signalPlayers();
	bool Director::anyActivePlayers();
	bool allSignalled_;
	std::mutex mut_;
	std::condition_variable directorCond_;

};

#endif /* defined DIRECTOR_H */