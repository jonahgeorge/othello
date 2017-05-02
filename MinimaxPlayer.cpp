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

int MinimaxPlayer::heuristic(OthelloBoard* b) {
  return b->count_score(this->symbol) - b->count_score(this->negate_symbol(this->symbol));
}

vector<BoardMove*> MinimaxPlayer::successor(OthelloBoard* b, char symbol) {
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

char MinimaxPlayer::negate_symbol(char s) {
  return (s == 'X') ? 'O' : 'X';
}

Node* MinimaxPlayer::minimax(OthelloBoard* board, bool maximizing_player, char symbol) {
  auto successors = successor(board, symbol);
  auto opponent_symbol = this->negate_symbol(symbol);

  if (successors.empty()) {
    auto value = this->heuristic(board);
    return new Node(NULL, value); 
  }

  if (maximizing_player) {
    // Maximizing Player
    auto best_value = new Node(NULL, -999); 

    for (vector<BoardMove*>::iterator bm = successors.begin(); bm != successors.end(); bm++) {
      auto v = this->minimax((*bm)->board, false, opponent_symbol); 

      if (v->score > best_value->score) 
        best_value = new Node((*bm)->move, v->score);
    }

    return best_value; 
  }

  {
    // Minimizing Player
    auto best_value = new Node(NULL, 999);

    for (vector<BoardMove*>::iterator bm = successors.begin(); bm != successors.end(); bm++) {
      auto v = this->minimax((*bm)->board, true, opponent_symbol); 

      if (v->score < best_value->score) 
        best_value = new Node((*bm)->move, v->score);
    }

    return best_value;
  }
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
  // Run minimax on tree
  auto result = this->minimax(b, true, this->symbol);

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
