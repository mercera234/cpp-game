# cpp-game
A video game that runs in a Windows command prompt written in C++ utilizing the pdcurses and boost libraries. Although the game is rendered in a command prompt, the intent is to be more sophisticated than a simple text-based adventure game. This game currently supports map traversal, battle mechanics, and menu navigation. 

This game is designed to utilize an input-event based engine. This means it is not truly animated, as their is no animation loop. This design allows me to spend more time working as a programmer and less as an graphic artist/animator. The game's main loop runs like this: draw the current state of the game, wait for user input, process the input, repeat.

# Project structure
There are currently four main parts of this project: PDCurseControls, RPGLib, GameLib, and Editors. The first three of these libraries are divided into 3 projects each: classes, automated unit-tests (*Tester), manual tests (*Tester - UI). 

Automated unit testing is done using the CppUnitTestFramework. There are currently hundreds of unit tests written so far. 
The manual test projects are for testing components that otherwise do not lend well to automated unit testing. This includes drawing components for visual inspection and working with resources external to the game code. 

# The projects
PDCurseControls is a library written to create higher-level drawable components such as textboxes, labels, menus, images, etc... This depends only on the pdcurses library itself. 

RPGLib is a library written to create generic RPG game elements for use in a command prompt video game. This depends on the PDCurseControls library.

GameLib is a library written specifically for the game I am creating. This depends on both RPGLib and PDCurseControls.

Editors is a separate application used for editing maps/images to be used in-game. 


# 3rd party components
The curses library is what is used to manipulate text in a command prompt window. pdcurses is an instance of the curses library ported to Windows among other operating systems. This project currently utilizes version 3.4.
https://pdcurses.sourceforge.io/

Boost is a set of libraries with a wide range of applications written entirely in C++.  
https://www.boost.org/
