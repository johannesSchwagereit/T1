# T1

TwixT is a boardgame by Alex Randolph.

T1 is a program to play TwixT by Johannes Schwagereit. It was created in 2003/2004 and is no longer being developed.


If you have any comments or ideas concerning T1, I would be very happy to hear from you.



Features
========

- Computer plays either left-right or top-down
- All links are placed automatically
- End of game is recognized by program
- Boardsize may be set to any value between 12x12 and 36x36
- Non-quadratic boards are possible (Handicap-games)
- Colors for both players may be set
- The window is resizeable
- Preferences are saved to file
- pie-rule is implemented
- optional diagonal 'area' lines (thanks to abraendle@gmx.de)
- Save and Load games
- Variable playing strength (by setting time for computer move)
- Undo and Redo of any number of moves

Missing Features
================

- Playing strength is still low
- Link removal is not implemented


Version History
===============

October 2004:
Version 0.6 *\<current version\>*  
Major update: Pie-Rule, improved GUI, improved playing strength


March 2004: 
Version 0.2b - Now compiles with Microsoft Visual C++.

June 2003:
Version 0.2 -
Bug-corrections and minor improvement of playing strength.

March 2003:
Version 0.1
A more or less are fully functional prototype. 
It played quite weak (but at least it beats www.twixt.de, the only 
other Twixt-program known to me).


This version of T1 is licenced under the GPL. (see LICENSE)

Technical details (version 0.2)
================================

- Operating System: Linux (Kernel 2.4.19)
- Programming language: C++
- Compiler: gcc (version 3.2).
- GUI toolkit: Fast Light Tool Kit (FLTK) Version 1.1.0

During execution, T1 does not read or write any file except the
property file: ~/.fltk/mail@johannes-schwagereit.de/T1.prefs

Installation
============
see INSTALL

