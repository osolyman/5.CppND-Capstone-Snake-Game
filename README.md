# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg


## The new Features

1. the player can enter a name and starts to play, and their score will be saved in a text file called "players_scores". This feature works as follows:
	* if the player enters a name that is already in the file, the program automatically takes the highst score, which could be the new one or the old one.
    * I have added std::async to launch an asynchronous task which is reading/writing in the file. (this should allow the game to keep running while 
      the score is being updated in the file in the background. 

2. I changed the food type to a shared pointer and added one new type of food, which is poison. this works as follows:
	* If the snake eats normal food, the score is increased by 1, and the speed is increased by 0.02.
    * If the snake eats poison food, the score is decreased by 1, and the speed is decreased by 0.02.
    * If the snake eats poison and its size is already one (just the head), the snake dies immediately.
    * The color of normal food is green, and the color of poison is orange.
    * The poison doesn't always have to appear.

3. I added new files obstacle.h and obstacle.cpp to add one obstacle to the game. It works as follows:
	* The obstacle only appears when the score is 8 or higher (for testing, you can reduce the number in obstacle.cpp).
    * Let's assume the score is 8 now, and the obstacle appears. If the snake eats poison and the score becomes 7, the obstacle should disappear again.
    * The snake dies immediately as soon as it eats the obstacle.
    * the color of the obstacle is red.

4. The last change is not a new feature.
	* for protecting the shared data (score) I have added mutex and lock_guard.

## covered Rubric Points

1. Loops, Functions, I/O:
	In file game.cpp, lines 135 - 207. in game.h declaration of functions and variables defined.
	* The project demonstrates an understanding of C++ functions and control structures.
    * The project reads data from a file and process the data, or the program writes data to a file.
    * The project accepts user input and processes the input.
    * The project uses data structures and immutable variables.

2. Object Oriented Programming:
	files obstacle.h and obstacle.cpp.
	* One or more classes are added to the project with appropriate access specifiers for class members.
    * Class constructors utilize member initialization lists.
    * Classes abstract implementation details from their interfaces.

3. Memory Management:
	for using shared pointers In file game.h (44-45). in game.cpp (15-16), every place in the code, the ( . ) changes to ( -> ).
    using references in function declarations: (game.cpp line 136 in function PlayerName()) and in (renderer.cpp line 41 in function Render(..)). 
	* The project makes use of references in function declarations.
    * The project uses smart pointers instead of raw pointers.

4. Concurrency:
	using mutex in (game.cpp function Update() line 89 and GetScore() lines 128-132).
    using async task in the execution in (game.cpp function HighScore() line 132-207). 
	* The project uses multithreading. (async tasks in the execution.)
    * A mutex or lock is used in the project.
 