/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include "MinimaxPlayer.h"

using std::vector;
using std::make_tuple;
using std::tuple;
using std::get;
using std::for_each;

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

int heuristic(OthelloBoard* b) {
  return b->count_score('O') - b->count_score('X');
}

vector<BoardMove*> successor(OthelloBoard* b, char symbol) {
  vector<BoardMove*> children;

  for (int c = 0; c < b->get_num_cols(); c++) {
    for (int r = 0; r < b->get_num_rows(); r++) {
      if (b->is_legal_move(c, r, symbol)) {
        auto move = new Move(c,r);
        auto board = new OthelloBoard(*b);
        board->play_move(move->col, move->row, symbol);
        children.push_back(new BoardMove(board, move));
      }
    }
  }

  return children;
}

Node* minimax(OthelloBoard* board, bool maximizing_player, char symbol) {
  auto successors = successor(board, symbol);
  auto opponent_symbol = (symbol == 'X') ? 'O' : 'X';

  if (successors.empty()) {
    auto value = heuristic(board);
    return new Node(NULL, value); 
  }

  if (maximizing_player) {
    // Maximizing Player
    auto best_value = new Node(NULL, -999); 

    for_each(successors.begin(), successors.end(), [board, &best_value, opponent_symbol](BoardMove* bm) mutable {
      auto v = minimax(bm->board, false, opponent_symbol); 

      if (v->score > best_value->score) 
        best_value = new Node(bm->move, v->score);
    });

    return best_value; 
  }

  {
    // Minimizing Player
    auto best_value = new Node(NULL, 999);

    for_each(successors.begin(), successors.end(), [board, &best_value, opponent_symbol](BoardMove* bm) mutable {
      auto v = minimax(bm->board, true, opponent_symbol); 

      if (v->score < best_value->score) 
        best_value = new Node(bm->move, v->score);
    });

    return best_value;
  }
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
  // Run minimax on tree
  auto result = minimax(b, true, this->symbol);

  // assign col and row
  col = result->move->col;
  row = result->move->row;
}

MinimaxPlayer::MinimaxPlayer(char symb) : Player(symb) {
}

MinimaxPlayer::~MinimaxPlayer() {
}

MinimaxPlayer* MinimaxPlayer::clone() {
  MinimaxPlayer* result = new MinimaxPlayer(symbol);
  return result;
}
