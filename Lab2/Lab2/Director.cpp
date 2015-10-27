#include "stdafx.h"
#include "Director.h"
#include "Utility.h"

#define START_POSITION 0
#define STRINGS_EQUAL 0
#define FIRST_OF_CONSECUTIVE 0
#define SECOND_OF_CONSECUTIVE 0

using namespace std;

class directorException : public exception
{
    virtual const char* badScriptFile() const throw()
    {
        return "Exception! Script file could not be opened.";
    }
} directorException;

Director::Director(string nameOfScriptFile, unsigned int playersToConstruct = 0)
{
    if (!Utility::doesFileExist(nameOfScriptFile))
    {
        throw directorException;
    }
    
    ifstream ifs(nameOfScriptFile);
    string sceneStr;
    const string sceneToken = "[scene]";
    vector<string> titlesOfScenes;
    vector<int> numPartConfigLines;
    numPartConfigLines.push_back(START_POSITION);
    unsigned int bestSum = START_POSITION;
    bool lastLineConfig = false;
    
    //this can be changed to fit the overall design of parsing strings better
    while (!ifs.eof() && ifs.good())
    {
        getline(ifs, sceneStr); //gets next line
        
        if (sceneStr.empty()) continue; //no need to do unneccesary work if the line is blank, so skip
        
        if (sceneStr.size() > sceneToken.size() && sceneToken.compare(sceneStr.substr(START_POSITION, sceneToken.size())) == STRINGS_EQUAL)
        {
            string sceneTitle = sceneStr.substr(sceneToken.size(), sceneStr.size() - sceneToken.size());
            titlesOfScenes.push_back(sceneTitle);
            lastLineConfig = false;
        }
        else
        {
            string configStr = sceneStr; //just for naming/ease of understanding
            
            if (!doesFileExist(configStr)) continue;
            
            ifstream configIfs(configStr); //configuration file
            
            string partDefLine;
            int numPartDefLines = START_POSITION;
            while (!configIfs.eof() && configIfs.good())
            {
                getline(configIfs, partDefLine);
                
                if (partDefLine.empty()) continue;
                numPartDefLines++;
            }
            
            numPartConfigLines.insert(numPartConfigLines.begin(), numPartDefLines);
            int sumVal = numPartConfigLines[FIRST_OF_CONSECUTIVE] + numPartConfigLines[SECOND_OF_CONSECUTIVE];
            if (sumVal > bestSum) bestSum = sumVal;
            
            if (lastLineConfig)
            {
                titlesOfScenes.push_back(""); //push back empty string
            }
            
            lastLineConfig = true;
        }
    }
    
    shared_ptr<Play> temp(new Play(ref(titlesOfScenes)));
    
    playSharedPointer = temp;
    
    int numPlayers = max(playersToConstruct, bestSum);
    
    for (int i = 0; i < numPlayers; i++)
    {
        shared_ptr<Player> player(new Player(playSharedPointer));
        playerContainer.push_back(move(player));
    }
}


Director::~Director()
{
}

void Director::cue()
{
    
}