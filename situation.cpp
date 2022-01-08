/***************************************************************************
                          situation.cpp  -  Representation of the board
                             -------------------
    begin                : Wed Oct 23 2002
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

#include "situation.h"

#include <iostream>

#include <FL/fl_ask.H>
#include <cstdlib>
#include <time.h>

#include "options.h"

extern options option;



crosses cross[4][9]; //any of the 4 direction of bridges may be crossed by 9 bridges




situation::situation(MainWindow* mainw){
  uchar i;
  mainwindow = mainw;
  //set up possible crossings
  //set up for UL
  cross[UL][0].xo = -1; cross[UL][0].yo =  1; cross[UL][0].dir = DL;
  cross[UL][1].xo =  0; cross[UL][1].yo =  2; cross[UL][1].dir = DH;
  cross[UL][2].xo =  1; cross[UL][2].yo =  2; cross[UL][2].dir = DH;
  cross[UL][3].xo =  0; cross[UL][3].yo =  1; cross[UL][3].dir = DH;
  cross[UL][4].xo =  0; cross[UL][4].yo =  1; cross[UL][4].dir = DL;
  cross[UL][5].xo =  1; cross[UL][5].yo =  1; cross[UL][5].dir = DH;
  cross[UL][6].xo =  1; cross[UL][6].yo =  1; cross[UL][6].dir = DL;
  cross[UL][7].xo =  0; cross[UL][7].yo = -1; cross[UL][7].dir = UH;
  cross[UL][8].xo =  1; cross[UL][8].yo =  0; cross[UL][8].dir = UH;
  // set up for DL
  for (i = 0; i<9; i++)
  {
    cross[DL][i].xo  = cross[UL][i].xo;
    cross[DL][i].yo  = - cross[UL][i].yo;
    cross[DL][i].dir = 3 - cross[UL][i].dir;
  }
  // set up for DH
  cross[DH][0].xo =  0; cross[DH][0].yo = -1; cross[DH][0].dir = UH;
  cross[DH][1].xo =  0; cross[DH][1].yo = -1; cross[DH][1].dir = UL;
  cross[DH][2].xo =  0; cross[DH][2].yo = -2; cross[DH][2].dir = UL;
  cross[DH][3].xo = -1; cross[DH][3].yo = -1; cross[DH][3].dir = UL;
  cross[DH][4].xo =  0; cross[DH][4].yo = -2; cross[DH][4].dir = UH;
  cross[DH][5].xo = -1; cross[DH][5].yo = -2; cross[DH][5].dir = UL;
  cross[DH][6].xo =  0; cross[DH][6].yo = -3; cross[DH][6].dir = UH;
  cross[DH][7].xo = -1; cross[DH][7].yo =  0; cross[DH][7].dir = DL;
  cross[DH][8].xo =  0; cross[DH][8].yo = -1; cross[DH][8].dir = DL;
  // set up for UH
  for (i = 0; i<9; i++)
  {
    cross[UH][i].xo  = cross[DH][i].xo;
    cross[UH][i].yo  = - cross[DH][i].yo;
    cross[UH][i].dir = 3 - cross[DH][i].dir;
  }

  // list of the 8 possible bridges starting at a given pin
  bridgeway[0].xo =  0; bridgeway[0].yo =  0; bridgeway[0].xd =  1; bridgeway[0].yd =  2;
  bridgeway[1].xo =  0; bridgeway[1].yo =  0; bridgeway[1].xd =  2; bridgeway[1].yd =  1;
  bridgeway[2].xo =  0; bridgeway[2].yo =  0; bridgeway[2].xd =  2; bridgeway[2].yd = -1;
  bridgeway[3].xo =  0; bridgeway[3].yo =  0; bridgeway[3].xd =  1; bridgeway[3].yd = -2;
  bridgeway[4].xo = -1; bridgeway[4].yo = -2; bridgeway[4].xd =  1; bridgeway[4].yd =  2;
  bridgeway[5].xo = -2; bridgeway[5].yo = -1; bridgeway[5].xd =  2; bridgeway[5].yd =  1;
  bridgeway[6].xo = -2; bridgeway[6].yo =  1; bridgeway[6].xd =  2; bridgeway[6].yd = -1;
  bridgeway[7].xo = -1; bridgeway[7].yo =  2; bridgeway[7].xd =  1; bridgeway[7].yd = -2;

  // set up the defensive moves
  defense[0].x = 4; defense[0].y =  0; defense[0].val = 0;  //Herkules
  defense[1].x = 2; defense[1].y =  0; defense[1].val = 0;  //Hades
  defense[2].x = 4; defense[2].y =  1; defense[2].val = 0;  //Achilles
  defense[3].x = 4; defense[3].y = -1; defense[3].val = 0;  //Achilles
  defense[4].x = 2; defense[4].y =  1; defense[4].val = 1;  //Zeus    more sense with bridge
  defense[5].x = 2; defense[5].y = -1; defense[5].val = 1;  //Zeus

  // set up the offensive moves
  offense[0].x = 2; offense[0].y =  1; offense[0].val = 1;  //direct, senseless w/o bridge
  offense[1].x = 2; offense[1].y = -1; offense[1].val = 1;  //direct
  offense[2].x = 1; offense[2].y =  2; offense[2].val = 1;  //direct
  offense[3].x = 1; offense[3].y = -2; offense[3].val = 1;  //direct
  offense[4].x = 3; offense[4].y =  1; offense[4].val = 0;  //Hermes
  offense[5].x = 3; offense[5].y = -1; offense[5].val = 0;  //Hermes
  offense[6].x = 2; offense[6].y =  2; offense[6].val = 0;  //Hektor
  offense[7].x = 2; offense[7].y = -2; offense[7].val = 0;  //Hektor
  offense[8].x = 4; offense[8].y =  0; offense[8].val = 0;  //Herkules
  offense[9].x = 2; offense[9].y =  0; offense[9].val = 0;  //Hades
  //without Hermes & Athene & Poseidon

  //set some starting positions  (for 1st pin with pierule and 24x24 board)
  startPos[0].x = 4;  startPos[0].y = 11;
  startPos[1].x = 4;  startPos[1].y = 10;
  startPos[2].x = 5;  startPos[2].y = 10;
  startPos[3].x = 5;  startPos[3].y =  9;
  startPos[4].x = 6;  startPos[4].y =  8;
  startPos[5].x = 7;  startPos[5].y =  7;
  startPos[6].x = 7;  startPos[6].y =  8;
  startPos[7].x = 8;  startPos[7].y =  7;
  startPos[8].x = 9;  startPos[8].y =  6;
  startPos[9].x = 4;  startPos[9].y = 12;
  
  lastmove_x = 0;
  lastmove_y = 0;
  setMoveNo(0);
  currentPlayer = YPLAYER;
}

situation::~situation(){
}

//String of Letters or Numbers
void situation::setOutStr(char st[3], uchar value, bool letter)
{
  if (letter)
    {
      uchar off = 0;
      if (value >= 26)
      {
        st[0] = 'A';
        off = 1;
      }

      st[off] = (char)value % 26 + 65;
      st[off + 1] = '\0';
    }
  else
    sprintf(st,"%d",value + 1);
}


/** Initialize situation for new game */
void situation::initialize(unsigned char cp){
  currentPlayer = cp;
  lastmove_x = 0;
  lastmove_y = 0;
  setMoveNo(0);
  memset(pin,    0, (MAXSIZE+4)*(MAXSIZE+4));  //    are set to 0
  memset(bridge, 0, (MAXSIZE+4)*(MAXSIZE+4)*4);  //    are set to 0
  control();
}


char situation::setPin(unsigned char x,unsigned char y,unsigned char player){
  //return: -1: not legal, 0: legal, but no bridge, >1: #bridges layed
  // set pin and lay bridges
  if (pin[x + 2][y + 2]) return -1; //not on already filled hole
  pin[x + 2][y + 2] = player;
  //set the 8 bridges, if possible
  char retval = 0;
  for (short i=0; i<8; i++)
    retval += setBridge(x,y,i,player);
  return retval;
}

bool situation::removePin(unsigned char x,unsigned char y,unsigned char player){
  // remove pin and bridges
  if (!pin[x + 2][y + 2]) return false; //only for already filled hole

  pin[x + 2][y + 2] = 0;
  //remove the 8 bridges, if possible
  for (short i=0; i<8; i++)
    removeBridge(x,y,i,player);
  return true;
}

char situation::setBridge(unsigned char x,unsigned char y,unsigned char dir, unsigned char player)
{
  // return: 0: no bridge, 1: bridge layed
  // set bridge and increment all 9 possible crossings and do all the checks
  x += 2 + bridgeway[dir].xo;
  y += 2 + bridgeway[dir].yo;
  //from here on modified x, y are used
  if (dir > 3) dir -= 4;

  //to lay a bridge, both pins have to have correct color, and bridge has to be allowed
  if (pin[x][y] != player || pin[x + bridgeway[dir].xd][y + bridgeway[dir].yd] != player ||
    bridge[x][y][dir])
  {
    return 0;
  }

  bridge[x][y][dir] = 100 + player;
  for (unsigned char i = 0; i<9; i++)
    bridge[x+cross[dir][i].xo] [y+cross[dir][i].yo] [cross[dir][i].dir] ++;
  return 1;
}

void situation::removeBridge(unsigned char x,unsigned char y,unsigned char dir, unsigned char player)
{
  // remove bridge and decrement all 9 possible crossings and do all the checks
  x += 2 + bridgeway[dir].xo;
  y += 2 + bridgeway[dir].yo;
  //from here on modified x, y are used
  if (dir > 3) dir -= 4;

  if (bridge[x][y][dir] > 99)
  {
    bridge[x][y][dir] = 0;
    for (unsigned char i = 0; i<9; i++)
      bridge[x+cross[dir][i].xo] [y+cross[dir][i].yo] [cross[dir][i].dir] --;
  }
  return;
}

unsigned char situation::neighbourVal(unsigned char x,unsigned char y,unsigned char dir)
 // get values of neighbours
{
  //int pval = value[x][y]; // old Version
  int pval = value[x][y] + ((pin[x][y]) ? 0 : 1); // new Version, 1 is added without pin
  if (dir <= 3)
    return (bridge[x][y][dir]>0 &&  bridge[x][y][dir]<90) ? NOCONNECTION : pval;
  else
  {
    int z = bridge[x + bridgeway[dir].xo][y + bridgeway[dir].yo][dir - 4];
    return (z > 0 && z < 90) ? NOCONNECTION : pval;
  }  
}

bool situation::connected(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
  //test if two pins are connected directly
{
  bool rv = false;
  unsigned char x3,y3;

  if (x1 > x2)  {x3=x1;y3=y1; x1=x2;y1=y2; x2=x3;y2=y3;} //swap
  if (abs(x1-x2) > 2 || abs(y1-y2) > 2 || abs(x1-x2) + abs(y1-y2) != 3)
    {return false;} //illegal

  if (y1-y2 ==-2) {if (bridge[x1][y1][0] > 99) rv = true;}
  else if (y1-y2 ==-1) {if (bridge[x1][y1][1] > 99) rv = true;}
  else if (y1-y2 == 1) {if (bridge[x1][y1][2] > 99) rv = true;}
  else if (y1-y2 == 2) {if (bridge[x1][y1][3] > 99) rv = true;}

  return rv;
}

bool situation::testBridge(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
  //test if two pins could be connected by a bridge (and not already connected)
{
  bool rv = false;
  unsigned char x3,y3;

  if (x1 > x2)  {x3=x1;y3=y1; x1=x2;y1=y2; x2=x3;y2=y3;} //swap
  if (abs(x1-x2) > 2 || abs(y1-y2) > 2 || abs(x1-x2) + abs(y1-y2) != 3)
    {return false;} //illegal

  if (y1-y2 ==-2) {if (bridge[x1][y1][0] == 0) rv = true;}
  else if (y1-y2 ==-1) {if (bridge[x1][y1][1] == 0) rv = true;}
  else if (y1-y2 == 1) {if (bridge[x1][y1][2] == 0) rv = true;}
  else if (y1-y2 == 2) {if (bridge[x1][y1][3] == 0) rv = true;}

  return rv;
}

short situation::evalStructure(unsigned char val, bool dir)
{
  unsigned char x1,y1,x2,y2;
  while (stackcnt > 0)
  {
    //handle top of stack
    stackcnt --;
    
    x1 = graphst_x[stackcnt];
    y1 = graphst_y[stackcnt];
    if (dir == XDIR)
    {
      if (x1 < smin)
        smin = x1;
    }
    else
    {
      if (y1 < smin)
        smin = y1;
    }
    value[x1][y1] = val;

    //add all the 8 neighbours
    x2 = x1 + 1; y2 = y1 - 2;
    if (connected(x1,y1,x2,y2) && value[x2][y2] > val)
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 + 2; y2 = y1 - 1;
    if (connected(x1,y1,x2,y2) && value[x2][y2] > val)
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 + 2; y2 = y1 + 1;
    if (connected(x1,y1,x2,y2) && value[x2][y2] > val)
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 + 1; y2 = y1 + 2;
    if (connected(x1,y1,x2,y2) && value[x2][y2] > val)
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}

    x2 = x1 - 1; y2 = y1 + 2;
    if (connected(x1,y1,x2,y2) && value[x2][y2] > val)
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 - 2; y2 = y1 + 1;
    if (connected(x1,y1,x2,y2) && value[x2][y2] > val)
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 - 2; y2 = y1 - 1;
    if (connected(x1,y1,x2,y2) && value[x2][y2] > val)
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 - 1; y2 = y1 - 2;
    if (connected(x1,y1,x2,y2) && value[x2][y2] > val)
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
  }
  return smin; //return smallest x or y found because re-check may be necessary
}

short situation::startStructure(short x,short y,unsigned char val, bool dir)
{ //eval a graph of pegs and bridges

//  std::cout << "startStructure: " << x << "-" << y << "\n";
  graphst_x[0] = (unsigned char) x;
  graphst_y[0] = (unsigned char) y;
  stackcnt = 1;
  smin = 100;
  return evalStructure(val, dir);
}


int situation::evaluate_x() // evaluate current situation as seen by XPLAYER (left-right)
{
  short i,j;
  short jmax = option.ysize+1;
  int res = 999;
  const unsigned char initval = 99;
  unsigned char tempval;
  
//  std::cout << "Eval_x\n";
  memset(value, initval, (MAXSIZE+4)*(MAXSIZE+4));  //    are set to 99
  //first column without corners
  for (j=3; j<jmax; j++)
    value[2][j] = (!pin[2][j]) ? 1 : 0;

  for (i=3; i<option.xsize+2; i++)      // x
    for (j=3; j<jmax; j++)    // y
    {
      if (pin[i][j] == YPLAYER)
        value[i][j] = 60; // this pin is occupied by opponent
      else                
      {
        // value is min of neighbours to the left
        tempval =
          MIN(MIN(neighbourVal(i-1,j-2,0),
                  neighbourVal(i-2,j-1,1)),
              MIN(neighbourVal(i-2,j+1,2),
                  neighbourVal(i-1,j+2,3)));
        if (pin[i][j]) // used by XPLAYER because Y already handled
        {
          if (value[i][j] > tempval)   // not already evaluated or now better?
            i = startStructure(i,j,tempval, XDIR); // to recognize backgoing structures
        }
        else //empty
        {
          if (i>3) tempval++; //if empty and not 2nd column
          value[i][j] = tempval;
        }
      }
    }
  //get smallest of all fields of last line
  for (j=3; j<jmax; j++)
    res = MIN(res, value[option.xsize+1][j]);
  return res;
}

int situation::evaluate_y() // evaluate current situation as seen by YPLAYER (top-bottom)
{
  short i,j;
  int res = 999;
  const unsigned char initval = 99;
  unsigned char tempval;

  memset(value, initval, (MAXSIZE+4)*(MAXSIZE+4));  // 36*36    are set to 99
  //first column without corners
  for (j=3; j<option.xsize+1; j++)
    value[j][2] = (!pin[j][2]) ? 1 : 0;

  for (j=3; j<option.ysize+2; j++)    // y
    for (i=3; i<option.xsize+1; i++)      // x
    {
      if (pin[i][j] == XPLAYER)
        value[i][j] = 60; // this pin is occupied by opponent
      else
      {
        // value is min of neighbours above
        tempval =
          MIN(MIN(neighbourVal(i-1,j-2,0),
                  neighbourVal(i-2,j-1,1)),
              MIN(neighbourVal(i+2,j-1,6),
                  neighbourVal(i+1,j-2,7)));
        if (pin[i][j]) // used by XPLAYER because Y already handled
        {
          if (value[i][j] > tempval)   // not already evaluated or now better?
            j = startStructure(i,j,tempval, YDIR); // to recognize backgoing structures
        }
        else //empty
        {
          if (j>3) tempval++; //if empty and not 2nd column
          value[i][j] = tempval;
        }
      }
    }
  //get smallest of all fields of last line
  for (j=3; j<option.xsize+1; j++)
    res = MIN(res, value[j][option.ysize+1]);
  return res;
}

int situation::evaluate(unsigned char thePlayer) // evaluate current situation
{
  int yval;
  int xval;
  
  evalNumber++; //one more situation evaluated

  // sequence is relevant only for debugging purposes
  if (thePlayer == YPLAYER)
  {
    xval = evaluate2_x();
    yval = evaluate2_y();
  }
  else
  {
    yval = evaluate2_y();
    xval = evaluate2_x();
  }

  
  //a winningmove was found?
  if (xval == 0 || yval == 0)
    winningMove = true;
  
  //return value for current position
  if (thePlayer == YPLAYER)
    return xval - yval;
  else
    return yval - xval;
}



void situation::computer() // find best move
{
  short bx = 0, by = 0, k;

  if (getMoveNo() == 0)  
  {
    if (option.pieRule)  //first move with pie-rule
    {
      if (option.xsize != 24 || option.ysize != 24)
        {bx = option.xsize / 4; by = option.ysize / 4;}
      else
      { //24-board
        srand((unsigned int)time((time_t *)NULL));
        short p = (rand()%10);
        bx = startPos[p].x - 1;
        by = startPos[p].y - 1;
        if (rand()%2) bx = option.xsize - bx - 1;
        if (rand()%2) by = option.ysize - by - 1;
        //std::cout << (int) p << " " << (int) bx  << "-" << (int) by << "\n";
        if (currentPlayer == XPLAYER)
          {k = bx; bx = by; by = k;} //swap
      }
    }
    else // firstmove is simple without pie-rule
    {
      bx = option.xsize / 2;
      by = option.ysize / 2;
    }
  }
  else if (getMoveNo() <= 3)   // simple answer with predefined moves
  {
    //find previous pin
    bx = moveHistory[getMoveNo()].x;
    by = moveHistory[getMoveNo()].y;
    if (currentPlayer == XPLAYER)
    {
//      if (1 /*getMoveNo() == 1 || getMoveNo() == 2 && (by <= 4 || by >= option.ysize - 5) */)
//      {
        if (by <= option.ysize / 2)
          by = ((option.ysize - by) / 2 + by);
        else
          by = by / 2;
//      }
//      else
//      {
//        by += (by >= option.ysize / 2) ? -4 : 4;
//      }
      if (bx <= 5)
        bx ++;
      if (bx >= option.xsize - 6)
        bx --;
    }
    else
    {
//      if (1 /*getMoveNo() == 1 || getMoveNo() == 2 && (bx <= 4 || bx >= option.xsize - 5) */)
//      {
        if (bx <= option.xsize / 2)
          bx = ((option.xsize - bx) / 2 + bx);
        else
          bx = bx / 2;
//      }
//      else
//        bx += (bx >= option.xsize / 2) ? -4 : 4;
      if (by <= 5)
        by ++;
      if (by >= option.ysize - 6)
        by --;
    }
    if (getMoveNo() == 2 || getMoveNo() == 3)
    {
      //check if  3rd move is really different from 1st
      if (abs(bx - moveHistory[getMoveNo() - 1].x) <= 2 &&
          abs(by - moveHistory[getMoveNo() - 1].y) <= 2)
      {
        //in this unlikely case an Ares (5/0) will improve my situation
        bx = moveHistory[getMoveNo() - 1].x;
        by = moveHistory[getMoveNo() - 1].y;
        if (currentPlayer == XPLAYER)
        {
          bx += (bx >= option.xsize / 2) ? -5 : 5;
        }
        else
        {
          by += (by >= option.ysize / 2) ? -5 : 5;
        }
      }
      if (getMoveNo() == 3 &&
          abs(bx - moveHistory[getMoveNo() - 2].x) <= 2 &&
          abs(by - moveHistory[getMoveNo() - 2].y) <= 2)
      {
        bx = 0; by = 0; // should be extremely seldom - take normal mechanism
      }
    }
  }
  if (bx + by == 0) // no simple move found yet
  {
    clock_t start; // Zeitmessung

    start = clock();
    startAlphaBeta();
    evaluate2_x();
    bx = bestX; by = bestY;
    printf("Elapsed: %.2f sec\n", ((double) (clock()-start)) / CLOCKS_PER_SEC);
  }

  
//  std::cout << "move: " << br << ":" << bx << "-" << by << "\n";
  setPin(bx,by,currentPlayer);
  lastmove_x = bx;
  lastmove_y = by;
}

void situation::update_gui() /* all the commands to redraw the window */
{
  mainwindow->redraw();
  mainwindow->lastmove->setLastmove(lastmove_x, lastmove_y);
  if (getMoveNo() > 1 || getMoveNo() > 0 && option.yhuman && option.xhuman)
    mainwindow->back_button->activate();
  else
    mainwindow->back_button->deactivate();
  if (getMaxMoveNo() > getMoveNo() && option.yhuman && option.xhuman ||
     getMaxMoveNo() > getMoveNo() + 1) //back-button has been pressed before
    mainwindow->forw_button->activate();
  else                
    mainwindow->forw_button->deactivate();
  Fl::check();  //force a redraw
}  

void situation::back_move()  /* undo last computer- and human-move */
{
  //return if not enough moves left
  if (getMoveNo()<1 || getMoveNo()<= 1 && (!option.yhuman || !option.xhuman))
    return;

  removePin(moveHistory[getMoveNo()].x,moveHistory[getMoveNo()].y,currentPlayer);
  setMoveNo(getMoveNo() - 1);
  if (!option.yhuman || !option.xhuman) //if one player is computerplayer
  {
    removePin(moveHistory[getMoveNo()].x,moveHistory[getMoveNo()].y,currentPlayer);
    setMoveNo(getMoveNo() - 1);
  }
  else
    switchCurrentPlayer();

  lastmove_x  = moveHistory[getMoveNo()].x;
  lastmove_y  = moveHistory[getMoveNo()].y;
  update_gui();
}

void situation::forward_move()  /* redo after undo last computer- and human-move */
{
  //return if not enough moves left
  if (getMaxMoveNo() <= getMoveNo() ||
      getMaxMoveNo() + 1 <= getMoveNo() && (!option.yhuman || !option.xhuman))
    return;

//  if (option.yhuman && option.xhuman) // both human
//     switchCurrentPlayer();
    
  setMoveNo(getMoveNo() + 1);
  setPin(moveHistory[getMoveNo()].x,moveHistory[getMoveNo()].y,currentPlayer);
  switchCurrentPlayer();
  if (!option.yhuman || !option.xhuman) //if one player is computerplayer
  {
    setMoveNo(getMoveNo() + 1);
    setPin(moveHistory[getMoveNo()].x,moveHistory[getMoveNo()].y,currentPlayer);
    switchCurrentPlayer();
  }
  lastmove_x  = moveHistory[getMoveNo()].x;
  lastmove_y  = moveHistory[getMoveNo()].y;
  update_gui();

}
  
void situation::handlePieRule()
{
  double x, y, di, da, swap;
  int answer;
  bool sb, swapping;

  if (!option.pieRule) return; //return if pierule is not used
  
  if  (currentPlayer == YPLAYER && option.yhuman || currentPlayer == XPLAYER && option.xhuman)
  { //let human decide for pie-rule
    answer = fl_choice("Pie Rule:\nDo you accept first pin or\ndo you demand a swap of sides?",
                       "Swap","Accept",NULL);
    if (answer == 0) //swap
      {
       sb = option.xhuman; option.xhuman = option.yhuman; option.yhuman = sb; //swap
      }
    return;
  }
  //else: the computer has to decide
  swapping = false;
  if (option.xsize + 4 <= option.ysize) // if diff between sides is >= 4 then take shorter 
  {
    swapping = (currentPlayer == YPLAYER); 
  }
  else if (option.ysize + 4 <= option.xsize) // if diff between sides is >= 4 then take shorter 
  {
    swapping = (currentPlayer == XPLAYER); 
  }
  else
  {      
    x = lastmove_x - option.xsize/2 - 0.5;  //normalize to middle of field
    y = lastmove_y - option.ysize/2 - 0.5;
    double xout = option.xsize/2 - 0.5 - 2;
    double xmid = option.xsize/2 - 0.5 - 4;
    //double xinn = option.xsize/2 - 0.5 - 6;
    //double yout = option.ysize/2 - 0.5 - 2;
    double ymid = option.ysize/2 - 0.5 - 4;
    double yinn = option.ysize/2 - 0.5 - 6;
    
    if (currentPlayer == YPLAYER) // currentPlayer is computer
      {swap = x; x = y; y = swap;} //swap
    di = x*x / (xmid * xmid) + y*y / (yinn * yinn); //inner ellipse of opening moves
    da = x*x / (xout * xout) + y*y / (ymid * ymid); //outer ellipse of opening moves
    //std::cout << "x: " << x << " y: " << y << " - " << di << " - " << da << "\n";
    swapping  = (di <= 1 || (da < 1 && (rand()%2)));
  }
  if (swapping)
    {
      fl_message("Pie Rule:\nComputer demands a swap.\nPlayer 1 is now Player 2 and vice versa.\nYour turn.");
      sb = option.xhuman; option.xhuman = option.yhuman; option.yhuman = sb; //swap
    }
  else
    fl_message("Pie Rule:\nComputer accepts first move.\nSides are not swapped.");
} 

void situation::control() //check next action, e.g. computer-move
{
  mainwindow->lastmove->setLastmove(lastmove_x, lastmove_y);
  if (getMoveNo() > 0)
    switchCurrentPlayer();

  if (getMoveNo() == 1)
  {
    update_gui();
    handlePieRule();
  }
  
  checkIfGameOver();

  computermove: //label for goto needed in pierule
  if  ((currentPlayer == YPLAYER && !option.yhuman ||
       currentPlayer == XPLAYER && !option.xhuman) && !option.gameOver)
  {
    mainwindow->cursor(FL_CURSOR_WAIT);
    update_gui();
    computerPlayer = (currentPlayer == YPLAYER && !option.yhuman) ? YPLAYER : XPLAYER;
    computer();
    mainwindow->cursor(FL_CURSOR_DEFAULT);
    
    switchCurrentPlayer();
    setMoveNo(getMoveNo() + 1);
    setMaxMoveNo(getMoveNo());  //set to same value to deactivate forw-button
    moveHistory[getMoveNo()].x = lastmove_x;
    moveHistory[getMoveNo()].y = lastmove_y;
    update_gui();
    checkIfGameOver();
    if (getMoveNo() == 1)
    {
      update_gui();
      handlePieRule();
      goto computermove;
    }
  }

    
  displayGameOver();
}

/** check If Game is Over */
void situation::checkIfGameOver(){
  if (evaluate_x() == 0)
    option.gameOver = 2;
  else                                                             
    if (evaluate_y() == 0)
      option.gameOver = 1;
}

/** display GameOver message */
void situation::displayGameOver(){
  if (option.gameOver)
  {
    update_gui();

    if (option.gameOver == 1)
    {
      mainwindow->cursor(FL_CURSOR_DEFAULT);
      fl_message("Game over.\nPlayer 1 has won.");
    }
    if (option.gameOver == 2)
    {
      mainwindow->cursor(FL_CURSOR_DEFAULT);
      fl_message("Game over.\nPlayer 2 has won.");
    }
  }
}
