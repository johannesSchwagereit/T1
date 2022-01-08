/***************************************************************************
          optionwindow.h  - set  option which can be changed during game
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Johannes Schwagereit
    email                : mail@johannes-schwagereit.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include "mainwindow.h"
#include <FL/Fl_Choice.H>


/**Window to set options
  *@author Johannes Schwagereit
  */

class optionWindow {
public: 
	optionWindow();
	~optionWindow();
  Fl_Window* pwin;
  Fl_Choice* choice1;
  Fl_Choice* choice2;
  Fl_Check_Button* check;
  Fl_Color colorList[9];
  bool letterVal;
  bool areaVal;
private:
  Fl_Menu_Item menu_Auswahl[9];
  unsigned short color1;
  unsigned short color2;
};

#endif
