SECTION 1

CSE 532 Fall 2015 Lab 1
Elom Kwame Worlanyo <elomworlanyo@wustl.edu>

PROGRAM OVERVIEW

Once again, a modular approach was taken in designing the code. The 
objective of this lab was to utilise a multithreaded approach to build a play 
script, given a configuration file with the following format:

Hamlet Prince of Denmark ACT II Scene II A room in the Castle 
King King.txt
Queen Queen.txt
Rosencrantz Rosencrantz.txt

The first line in this config file would be the title of the play, with the 
following lines having the format {character} {character config file}. 
Again, each character only had a single name as a word, since the program
instructions told us to ignore any line with more than 2 whitespace delimited 
words.

Character config files were relatively straightforward, consisting of a number,
then some text on a line.

The major heavy lifting in this approach comes in the main file, which utilises
the Play and Player classes defined in the Play.cpp and Player.cpp files.
All concurrent activity was run from the Player class, with it asynchronously
reading in each player script fragment and storing it in a private container.
The Play class was refactored to simply use a counter to coordinate the 
recitation of lines.

The primary wrapper facade used in this lab was the use of a condition variable
to synchronize waiting threads within the recite method of the Play class. 
As noted, the counter object was used with this approach to synchronize the 
output of the lines. One interesting issue I came across was the Player
objects going out of scope initially when being pushed into the vector of
Players. This was solved by paying closer attention to the scope.


SECTION 2

1. Unzip the attached Lab1 and Lab1Extra zip folders with any appropriate 
unzipping tool available on the CEC machines, such as 7zip or winrar. 
Each unzipped folder should contain the source and header files necessary to 
build the respective solutions.
2. Start a new project in Visual studio 2013 for a given unzipped folder, and 
add the source and header files to each project using the 'add existing
item' prompt. Afterwards, you should be able to build the project. Make sure
stadfx.h is present
3. Run either project by going to the Debug folder and running the executable,
along with the program inputs for each project as specified on 
www.cse.wustl.edu/~cdgill/courses/cse532/lab1


SECTION 3

This solution was evaluated mainly with variations of the sample code given
in the prompt, as well as with files used by the professor to evaluate lab0,
since I believed it was of enough length to draw out any
races and edge cases. some cases were created using the tool developed in extra
credit too. Each variation was again run between 20 - 40 times, using a 
shell script to automate this. The first variant was purely what had already
been given, which passed in due time. The second variant consisted of adding
and subtracting characters. The maximum number of characters and hence threads
tested was around 15 {Text used was still the same, only new character names,
config files, and line numbers were added}. Minimum was 1. All passed muster.
Malformed inputs were also tested, and of particular interest was what would 
happen if script fragments had lines in different orders. The program handled
this well, with the output being as expected. There were however two related 
cases of malformed issue which were not handled by the program. First case was
when the script fragments purported to have 2 lines at the same line in the 
play. Assuming all lines in the sequence between the first line and the last 
line of the play were represented, then as per the directions, we noted this 
issue and output a warning in cerr. I.e. if lines 1,2,2,3 were given, we'd note
this and the program would continue as expected. If, however, lines 1,2,2,4 were
given, the counter would wait forever since it would never be incremented past
3, seeing as line 3 is not present in the script fragments. Indeed, in any 
script fragment which had a malformed line with a line number such that there 
was a gap between line numbers, the program ended up waiting indefinitely. For
example if a line number given was greater than the total number of lines
present. This seemed to be beyond the scope of the lab and as such was not 
addressed. Sharing information between Players, however, would probably be the 
best method to fix this issue.





EXTRA CREDIT

Tested using sample code given, as well as several variants on aformentioned
sample code. Approach used was primarily sequential, with the program
reading in lines, storing the characters in a set to preserve uniqueness, and
utilising the Line struct already created in the previous exercise to 
store the relationship between a line, its number, and the character that 
owned it. The scramble portion used a time-based seed and the random_shuffle
algorithm to pseudo-randomly shuffle the contents of the line vector and store
them in the appropriate file.