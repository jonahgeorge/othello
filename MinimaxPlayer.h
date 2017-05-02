/*
 * MinimaxPlayer.h
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */

#ifndef MINIMAXPLAYER_H
#define MINIMAXPLAYER_H

#include "OthelloBoard.h"
#include "Player.h"
#include <vector>

using std::vector;

class Move {
  public:
    int col;
    int row;

    Move(int c, int r) {
      this->col = c;
      this->row = r;
    }
};

class Node {
  public:
    Move* move;
    int score;

    Node(Move* m, int s) {
      this->move = m;
      this->score = s;
    }
};

class BoardMove {
  public:

    Move* move;
    OthelloBoard* board;

    BoardMove(OthelloBoard* board, Move* move) {
      this->board = board;
      this->move = move;
    }
};

/**
 * This class represents an AI player that uses the Minimax algorithm to play
 * the game intelligently.
 */
class MinimaxPlayer : public Player {
  public:

    /**
     * @param symb This is the symbol for the minimax player's pieces
     */
    MinimaxPlayer(char symb);

    /**
     * Destructor
     */
    virtual ~MinimaxPlayer();

    /**
     * @param b The board object for the current state of the board
     * @param col Holds the return value for the column of the move
     * @param row Holds the return value for the row of the move
     */
    void get_move(OthelloBoard* b, int& col, int& row);

    /**
     * @return A copy of the MinimaxPlayer object
     * This is a virtual copy constructor
     */
    MinimaxPlayer* clone();

  private:
    int heuristic(OthelloBoard* b);
    vector<BoardMove*> successor(OthelloBoard* b, char symbol);
    Node* minimax(OthelloBoard* board, bool maximizing_player, char symbol);
    static char negate_symbol(char s);
};

#endif
