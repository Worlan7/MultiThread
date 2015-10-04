SECTION 1

CSE 532 Fall 2015 Lab 0
Elom Kwame Worlanyo <elomworlanyo@wustl.edu>

PROGRAM OVERVIEW

A modular approach was taken in designing the code as much as possible. The 
objective of this lab was to utilise a multithreaded approach to build a play 
script, given a configuration file with the following format:

Hamlet Prince of Denmark ACT II Scene II A room in the Castle 
King King.txt
Queen Queen.txt
Rosencrantz Rosencrantz.txt

The first line in this config file would be the title of the play, with the 
following lines having the format {character} {character config file}. I noted
that each character only had a single name as a word, since the program
instructions told us to ignore any line with more than 2 whitespace delimited 
words.

Looking at the sample output file, It is also interesting to note that every
Character has a period and line break after they are highlighted in the script.

Character config files were relatively straightforward, consisting of a number,
then some text on a line.

The major heavy lifting in this approach comes in the main file, which utilises
the Play class defined in the Play.cpp files. Since this was a multithreaded
approach, the list of lines stored by the Play class had to be protected
to keep threads from deadlocking or having adverse effects from races.

The main Wrapper Facade utilised to prevent this was scoped locking, where 
a std::lock_guard initialized with a mutex held by a Play object was used
to give/deny/secure access to this list of lines stored by the Play class.
An interesting error I came accross while using this approach was the fact
that passing in an already opened ifstream to the thread function that
handled each character part still lead to deadlock, evene when passed by 
reference. A brand new ifstream had to be constructed and passed before
this approach bore any fruit.


SECTION 2

1. Unzip the attached Lab0 and Lab0Extra zip folders with any appropriate 
unzipping tool available on the CEC machines, such as 7zip or winrar. 
Each unzipped folder should contain the source and header files necessary to 
build the respective solutions.
2. Start a new project in Visual studio 2013 for a given unzipped folder, and 
add the source and header files to each project using the 'add existing
item' prompt. Afterwards, you should be able to build the project. Make sure
stadfx.h is present
3. Run either project by going to the Debug folder and running the executable,
along with the program inputs for each project as specified on 
www.cse.wustl.edu/!cdgill/courses/cse532/lab0


SECTION 3

This solution was evaluated mainly with variations of the sample code given
in the prompt, since I believed it was of enough length to draw out any
races and edge cases. some cases were created using the tool developed in extra
credit too. Each variation was run between 20 - 40 times, using a 
shell script to automate this. The first variant was purely what had already
been given, which passed in due time. The second variant consisted of adding
and subtracting characters. The maximum number of characters and hence threads
tested was around 15 {Text used was still the same, only new character names,
config files, and line numbers were added}. Minimum was 1. All passed muster.
Malformed inputs were also tested, and of particular interest was what would 
happen if 2 different character config files purported to have text at the same
line. This was not handled by the program, since different outputs could happen
based on the same input. i.e. if King and Queen both said they had lines 25, 
sometimes King would have 25 while other times Queen would. The program reacted
as expected to other malformed inputs tested as per the directions on the lab
page.





EXTRA CREDIT

Tested using sample code given, as well as several variants on aformentioned
sample code. Approach used was primarily sequential, with the program
reading in lines, storing the characters in a set to preserve uniqueness, and
utilising the Line struct already created in the previous exercise to 
store the relationship between a line, its number, and the character that 
owned it