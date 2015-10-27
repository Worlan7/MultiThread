#pragma once
#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "Play.h"
#include "Player.h"

class Director
{
public:
    Director(std::string nameOfScriptFile, unsigned int playersToConstruct);
    ~Director();
    void cue();
private:
    std::shared_ptr<Play> playSharedPointer;
    vector<std::shared_ptr<Player>> playerContainer;
    //?Script directorScript;
};

#endif /* defined DIRECTOR_H */