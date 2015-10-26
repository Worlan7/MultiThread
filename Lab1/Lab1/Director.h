#pragma once
#if ! defined (DIRECTOR_H)
#define DIRECTOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Play.h"
#include "Player.h"

class Director
{
public:
    Director(std::string nameOfScriptFile, unsigned int playersToConstruct);
    ~Director();
    void cue();
private:
    shared_ptr<Play> playSharedPointer;
    vector<shared_ptr<Player>> playerContainer;
    //?Script directorScript;
};

#endif /* defined DIRECTOR_H */