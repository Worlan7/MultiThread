#pragma once

#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <sstream>
#include <fstream>

enum numConstants : int
{
	ZERO = 0,
	ONE = 1,
	TWO = 2,
};

enum programErrors : int
{
	runningFine = 0,
	notEnoughArgs = 1,
	fileNotOpened = 2,
	noValidCharDef = 3,
	wrongArgs = 4,
};

struct Line
{
	//constructors 
	Line(){};
	Line(int num, std::string character, std::string txt)
		: lineNumber(num), lineCharacter(character), lineText(txt) {};
	//for comparison
	bool operator< (const Line &) const;
	//member variables
	int lineNumber;
	std::string lineCharacter;
	std::string lineText;
};

struct Part
{
	Part(){};
	Part(std::string name, std::string file) : characterName(name),
		fileName(file){};
	//member variables
	std::string characterName;
	std::string fileName;
};

struct Fragment
{
	std::vector<std::shared_ptr<Part>> parts;
};

struct Script
{
	std::vector<std::shared_ptr<Fragment>> fragments;
};

struct Message
{
	//Used to pass special messages where no parts are required
	Message(bool _endOfPlay) : endOfPlay(_endOfPlay){};
	//Used to transmit messages to Player's active queue
	Message(bool _endOfPlay, Part _inputPart, int _sceneFragmentNum) : 
		endOfPlay(_endOfPlay), sceneFragmentNum(_sceneFragmentNum),
		inputPart(_inputPart){};
	
	//member variables
	bool endOfPlay;
	int sceneFragmentNum;
	Part inputPart;
};

template<typename T>
class PlayerQueue
{
private:
	mutable std::mutex mut;
	std::queue<std::shared_ptr<T>> dataQueue;
	std::condition_variable dataCond;
public:
	PlayerQueue(){}
	std::shared_ptr<T> waitAndPop()
	{
		std::unique_lock<std::mutex> lk(mut);
		dataCond.wait(lk, [this] {return !dataQueue.empty(); });
		std::shared_ptr<T> result = dataQueue.front();
		dataQueue.pop();
		return result;
	}

	void push(T newValue)
	{
		std::shared_ptr<T> data(std::make_shared<T>(std::move(newValue)));
		std::lock_guard<std::mutex> lk(mut);
		dataQueue.push(data);
		dataCond.notify_one();
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return dataQueue.empty();
	}
};


class invalidOnStageException : public std::exception{
public:
	const char * what() const throw()
	{
		return "Invalid Players on Stage Exception\n";
	}
};

class invalidSceneEnterException : public std::exception{
public:
	const char * what() const throw()
	{
		return "Invalid Scene Entry Fragment Exception\n";
	}
};

class directorException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Exception! Script file could not be opened.";
	}
};


