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

struct Node {
  int col;
  int row;
  float value;
  
  Node() {
  }

  Node(int col, int row, float value) {
    this->col = col;
    this->row = row;
    this->value = value;
  }
};

int heuristic(OthelloBoard* b, char symbol) {
  // TODO
  // (number of this->symbol compared to "other")
  return b->count_score(symbol);
}

// vector<OthelloBoard*> successor(OthelloBoard* b, char symbol) {
//   vector<OthelloBoard*> children;
// 
//   for (int c = 0; c < b->get_num_cols(); c++) {
//     for (int r = 0; r < b->get_num_rows(); r++) {
//       if (b->is_legal_move(c, r, symbol)) {
//         // clone board
//         auto clone = new OthelloBoard(*b);
// 
//         // flip pieces on clone
//         clone->play_move(c, r, symbol);
// 
//         // add new board to children
//         children.push_back(clone);
//       }
//     }
//   }
// 
//   return children;
// }

vector<Node*> successor(OthelloBoard* b, char symbol) {
  vector<Node*> children;

  for (int c = 0; c < b->get_num_cols(); c++) {
    for (int r = 0; r < b->get_num_rows(); r++) {
      if (b->is_legal_move(c, r, symbol)) {
        auto child = new Node(c,r,0);
        children.push_back(child);
      }
    }
  }

  return children;
}

// TODO Fix heuristic
// TODO Return row,col from minimax
// TODO Fix symbol passing into minimax
Node* minimax(OthelloBoard* parent, bool maximizing_player, char symbol) {
  auto children = successor(parent, symbol);

  if (children.empty()) {
    auto v = heuristic(parent, symbol);
    return new Node(0,0,v);
  }

  if (maximizing_player) {
    float best_value = -9999999;
    std::for_each(children.begin(), children.end(), [&best_value, symbol](OthelloBoard* child) mutable {
      auto v = minimax(child, false, (symbol == 'X') ? 'O' : 'X');
      best_value = std::max(best_value, v->value);
    });
    return new Node(0,0,best_value);
  } else {
    float best_value = 9999999;
    std::for_each(children.begin(), children.end(), [&best_value, symbol](OthelloBoard* child) mutable {
      auto v = minimax(child, true, (symbol == 'X') ? 'O' : 'X');
      best_value = std::min(best_value, v->value);
    });
    return new Node(0,0,best_value);
  }
}

MinimaxPlayer::MinimaxPlayer(char symb) : Player(symb) {
}

MinimaxPlayer::~MinimaxPlayer() {
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
  // Run minimax on tree
  auto r = minimax(b, true, this->symbol);
  std::cout << r->value << std::endl;
  exit(-1);

  // assign col and row
}

MinimaxPlayer* MinimaxPlayer::clone() {
  MinimaxPlayer* result = new MinimaxPlayer(symbol);
  return result;
}
