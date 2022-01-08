/***************************************************************************
                          situation.h  -  Representation of the board
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

#ifndef SITUATION_H
#define SITUATION_H

#include <FL/Fl.H>
#include "mainwindow.h"


#define EMPTY 0
#define XPLAYER 1
#define YPLAYER 2
#define MAXSIZE 36
#define GRAPHSTACKSIZE 200
#define NOCONNECTION 60
#define MAXMOVES 200

//direction of bridges
#define UH 0 //up high  (2 nach oben, 1 nach rechts)
#define UL 1 //up low  (1 nach oben, 2 nach rechts)
#define DL 2 //down low  (1 nach unten, 2 nach rechts)
#define DH 3 //down high  (2 nach unten, 1 nach rechts)

#ifdef __GNUC__
  #define MIN(a,b) ((a) <? (b))
#else
  #define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
  
struct crosses //Possible crossing bridges of a bridge
{
  char xo; //x-offset for one end of the crossing bridge
  char yo; //y-offset
  unsigned char dir; //direction
};


struct bridgeways //Direction for a bridge to lay
{
  char xo; //x-offset of pin if starting point is different
  char yo; //y-offset
  char xd; //direction of pin;
  char yd;
};

#define MOVELISTLENGTH 6
struct move
{
  char x;
  char y;
  int  val;
};


/**Representation of the board with all connections
  *@author Johannes Schwagereit
  */

class situation {
private: //data
  unsigned char pin[MAXSIZE+4][MAXSIZE+4];  //0=empty, 1=XPLAYER, oder 2=YPLAYER
  unsigned char bridge[MAXSIZE+4][MAXSIZE+4][4]; //bridges nach rechts vom aktuellen pin, s.o.

  //used in evaluate2
  unsigned char flags[MAXSIZE+4][MAXSIZE+4];  
  bridgeways bridgeway[8]; //the 8 possible bridges from 1 point
  
  move          startPos[10]; // some starting positions with pierule and 24 * 24 board
  unsigned char lastmove_x;
  unsigned char lastmove_y;
  unsigned short moveNo; //current number of move
  unsigned short maxMoveNo; //max. move-number, only relevant for back-/Forw.-buttons
  // MAXSIZE is max, but MAXSIZE+4 is used to simplify range-checking
  unsigned char currentPlayer; //Next player to set pin ( XPLAYER or YPLAYER )
  unsigned char computerPlayer; //player played by computer ( XPLAYER or YPLAYER )
  unsigned char movingPlayer; //player to set pin during alphsbeta ( XPLAYER or YPLAYER )
  bool          winningMove;
  int           evalNumber; //number of evalutations
  MainWindow*   mainwindow;

  move          topMove[199]; //list of Moves on top-layer
  short         topMoveI; // index for critMoves
  move          critMove[199]; //list of critical Moves
  short         critMoveI; // index for critMoves
  short         possMoveI; // index for possMoves
  move          depthMove[99]; //list of tested moves in higher depths
  unsigned char bestX; //best move found
  unsigned char bestY; //best move found
  move          llMove[MOVELISTLENGTH][2]; //lowlevelmoves for depth 1 one 2
  move          killer[8][2]; //stored moves for killerheuristic - 2 for each level
  unsigned char killerI[8];
  
  void          addToMoveCrit(short x, short y, int upleft);

  move          defense[6]; //the defensive moves
  move          offense[10]; //the offensive moves
  void          addToMovePoss(short x, short y, int upleft, move possMove[]);
  void          addDefense(unsigned char thePlayer, move possMove[]);
  void          addOffense(unsigned char thePlayer, move possMove[]);
  void          criticalSingle(unsigned char thePlayer, short x, short y);
  int           alphaBeta(short depth, int alpha, int beta, unsigned char player);
  void          startAlphaBeta();
  short         generateMoves(unsigned char player, move possMove[], short depth);
//  short         testMoves(short depth, unsigned char player, move possMove[]);
  short         getMoves(short depth, unsigned char player, move possMove[]);
  unsigned char smin;
    
private: //functions
  char          setBridge(unsigned char x,unsigned char y,unsigned char dir, unsigned char player);
  void          removeBridge(unsigned char x,unsigned char y,unsigned char dir, unsigned char player);
  int           evaluate_x(); //for Xplayer
  int           evaluate_y(); //for Yplayer
  int           evaluate2_x(); //for Xplayer
  int           evaluate2_y(); //for Yplayer
  //int           evaluate3_y(); //for Yplayer
  int           critical_x(int); //for Xplayer
  int           critical_y(int); //for Xplayer
  bool          connected(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
  bool          testBridge(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
  short         evalStructure(unsigned char val, bool); //eval a graph of pegs and bridges
  short         startStructure(short x,short y,unsigned char val, bool);
  short         evalStructureCrit(bool); //eval a graph of pegs and bridges
  short         startStructureCrit(short x,short y, bool);
  unsigned char graphst_x[GRAPHSTACKSIZE];
  unsigned char graphst_y[GRAPHSTACKSIZE];
  short         stackcnt;
  void          putCrit(short x1, short y1, short m1, short x2, short y2, short m2, bool);
  
public: //data      eigentlich auch private
  unsigned char value[MAXSIZE+4][MAXSIZE+4];  //used for evaluation of board
  unsigned char crit[MAXSIZE+4][MAXSIZE+4];   //used to find critical path (or net)
  move          moveHistory[299]; //list of Moves played
public: //functions
  situation(MainWindow* mainw);
  ~situation();
  unsigned char neighbourVal(unsigned char x,unsigned char y,unsigned char dir);
  unsigned char neighbourVal2(unsigned char x,unsigned char y,unsigned char, unsigned char);
  int           evaluate(unsigned char thePlayer);
  //int           evaluate2(unsigned char thePlayer);
  void          computer(); // computer tries to find best move
  void          control(); // check next action, e.g. computer-move
  unsigned char getPin(unsigned char x,unsigned char y) {return pin[x + 2][y + 2];}
  char          setPin(unsigned char x,unsigned char y,unsigned char player);
  bool          removePin(unsigned char x,unsigned char y,unsigned char player);
  void          removePinFast(unsigned char x,unsigned char y) {pin[x + 2][y + 2] = 0;}
  unsigned char getBridge(unsigned char x,unsigned char y,unsigned char dir) {return bridge[x+2][y+2][dir];}
  unsigned char getCurrentPlayer() {return currentPlayer;}
  void          switchCurrentPlayer() {currentPlayer = 3 - currentPlayer;}  // switch between 1 and 2
  void          initialize(unsigned char cp);  /* Initialize situation for new game */
  void          back_move();  /* undo last computer- and human-move */
  void          forward_move();  /* redo after undo last computer- and human-move */
  void          update_gui(); /* all the commands to redraw the window */
  static void   setOutStr(char st[3], uchar value, bool letter);
  void          setLastMove(unsigned char x,unsigned char y) {lastmove_x = x; lastmove_y = y;}
  unsigned short getMoveNo () {return moveNo;}
  void           setMoveNo (unsigned short newMoveNo) {moveNo = newMoveNo;}
  unsigned short getMaxMoveNo () {return maxMoveNo;}
  void           setMaxMoveNo (unsigned short newMaxMoveNo) {maxMoveNo = newMaxMoveNo;}
  void           handlePieRule();
  /** check If Game is Over */
  void checkIfGameOver();
  /** display GameOver message */
  void displayGameOver();
  bool           checkPattern(unsigned char thePlayer, unsigned char x,unsigned char y,int dir,
                    move possMove[], bool offensive); //implemented in pattern.cpp
  void loadGame(); //implemented in file.cpp
  void saveGame(); //implemented in file.cpp
};

#endif
