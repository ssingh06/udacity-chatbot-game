# CPPND: Game Chatbot

This is the capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213): Game Chatbot
This projects utilizes the Memory Management Chatbot program from the third course in the C++ Nanodegree and modifies it to let the user play a simple Trivia Game.

<img src="images/chatbot_demo.gif"/>

The ChatBot code creates a dialogue where users can ask questions about some aspects of memory management in C++. After the knowledge base of the chatbot has been loaded from a text file, a knowledge graph representation is created in computer memory, where chatbot answers represent the graph nodes and user queries represent the graph edges. After a user query has been sent to the chatbot, the Levenshtein distance is used to identify the most probable answer. The code uses raw pointers to represent the knowledge graph and interconnections between objects throughout the project.

The program works in the following three modes:
1. Manage: In this mode, the user can query data about the Chatbot itself. The Chatbot asks the credentials of the user before letting them access this mode and then can provide basic information about the Chatbot. The user is required to enter a password (default password: 'password') in order to get access to this mode.

In the current implementation, the user can get the following information about the chatbot in this mode:
    * Trivia High Scores (across all the times that the game was played on this machine)
    * Trivia total number of questions served to the user. (across all the times that the game was played on this machine)
    * Update Password
2. C++ Memory Model: This is the original functionality from the Membot of the third course. The chatbot continues to answer questions about C++ Memory Model.
3. Games: This mode allows the user to choose one of the interactive games within the Chatbot. For now, there is only one game here: Trivia. The trivia game offers a basic flow of control:
    * User chooses 'Trivia' under 'Games'.
    * User is shown the current score and prompted to proceed to see the next question.
    * Chatbot shows the next question to the user.
    * The user can enter an answer.
    * Chatbot compares the user input with the correct answer and shows the result to the user.
    * If the answer was right, the score is updated and shown to the user before the user proceeds to the next question. 


## Dependencies for Running Locally
* cmake >= 3.11
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
* wxWidgets >= 3.0
  * Linux: `sudo apt-get install libwxgtk3.0-dev libwxgtk3.0-0v5-dbg`
  * Installation instructions can be found [here](https://wiki.wxwidgets.org/Install). Some version numbers may need to be changed in instructions to install v3.0 or greater.
* libcurl >= 4.0
  * Linux: `sudo apt-get install libcurl4-openssl-dev`
* libjsoncpp
  * Linux: `sudo apt-get install libjsoncpp-dev`

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`. To build with debug flags `cmake -DCMAKE_BUILD_TYPE=Debug .. && make`
4. Run it: `./chatbot`.

## Design Details
To add the Trivia Game functionality, the program talks to the REST API interface provided by [jService](http://jservice.io/) to allow the user to interactively play the Trivia game. The user can retrieve new questions, answer them and keep track of their score as they go along.

The code is organized in the following directories which map to logically different sub-features:
1. game: All game related classes and features are grouped under this directory.
2. game/trivia: This directory contains an implementation of the abstract class 'Game'.
3. graph: This directory contains the graph classes used by the chatbot.
4. http: Implementation of http client and rest api classes.
5. manage: This directory encapsulates the management mode feature of the chatbot.
6. util: Basic utility features that can be used by any of the above features. Eg. Message queue, json parser.

## Additional Rubrics Addressed
### Loops, Functions, I/O
1. The ChatbotManager class reads and writes info to a transient file 'running_config.bin' in order to store info about the Chatbot when the program is shutdown. When the program starts next time, this file is read and the info is loaded into a map to service user requests. This file deleted after the program startup is complete and would only be created again right before the shutdown to store the update Chatbot information.
2. There has been a conscious effort to maintain and breakdown code into logically separated files, classes and functions. All the above directories contain classes and the classes contain function that satisfy this rubric.
3. The Chatbot basic functionality inherently is based on accepting user input and processing it generate a response from the Chatbot.

### Object Oriented Programming
1. With the exception of header files containing constant variable definitions, the entire project is organized into classes.
2. All classes in the project, define appropriate access specifiers for member variables and functions.
3. The constructors for the game, http_client use initialization lists.
4. As explained in the design details, the directories and the classes are divided in a way to encapsulated logical functionality.
5. The game directory and the trivia subdirectory are examples of using inheritance, abstract classes and virtual functions with overriden functions encapsulating special logic in derived classes.
6. The message_queue.h file uses templatized functions in order to accept generic messages from the classes using the message queue.

### Concurrency
1. The message queue, http client and the rest api classes use multiple threads.
2. Http client and rest api classes use future and promise to pass data between threads.
3. Message queue makes use of mutex and unique_lock.
4. Message queue uses a condition variable to synchronize communication between producer and consumer threads.

## Bugs
There are some bugs in the current implementation of the program which do not hamper the overall functionality, but definitely make UX rough. The bugs listed below the improvement items listed in the last section will improve the UX and will make the program more robust with respect to different user interaction patterns. 
1. All node transitions happen based on the Levenshtein algorithm. This leads to faulty transitions sometimes, especially where transitions must be made by making exact matches with the user input.
2. The 'Manage' mode offers to store passwords, which are stored in cleartext files. These files must be encrypted or there must be database used to store persistent data.
3. Sometimes, [jService](http://jservice.io/) returns questions with empty question strings. The program doesn't show error in that case.
4. The program tries to update the Chatbot program background based on the mode that the user is in presently, but the render() method in chatgui() doesn't redraw the entire frame and hence the background is updated as the user continues interacting with the Chatbot in a particular mode.

## Next Tasks
The program is written in a way to be able to easily extend the functionality under all the three modes. Some future ideas to extend the project could be:
1. Add new games to the Chatbot using some of the popular REST API interfaces. (e.g. [Pokemon](https://pokeapi.co/), [Advices](https://api.adviceslip.com/))
2. Prefetch and cache data from the REST APIs and clean up the cached data once it has been serviced to the user. 
3. Change the answergrapgh.txt to specify graph parameters in json format.
4. Show the number of correct answers in Trivia stats under Manage mode. 
5. Refactor the Graph management within the program to move the separate the chat and graph functionality.
6. There are too many inputs needed from the user such as (Yes/No). This can be improved by eliminating some of these questions and directly transitioning to the next logical state in the flow.
