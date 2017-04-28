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

class Move {
  public:
  int col;
  int row;

  Move();
  Move(int c, int r);
  void print();
};

Move::Move() {
}

Move::Move(int c, int r) {
  this->col = c;
  this->row = r;
}

void Move::print() {
  std::cout << "Move: " << this->col << "," << this->row << std::endl;
}

class Node {
  public:
  Move* move;
  int score;

  Node();
  Node(Move* m, int s);
  void print();
};

Node::Node() {
}

Node::Node(Move* m, int s) {
  this->move = m;
  this->score = s;
}

void Node::print() {
  this->move->print();
  std::cout << "Score: " << this->score << std::endl;
}

int heuristic(OthelloBoard* b, char symbol, char opponent_symbol) {
  return b->count_score(symbol) / 
    b->count_score(opponent_symbol);
}

vector<Move*> successor(OthelloBoard* b, char symbol) {
  vector<Move*> children;

  for (int c = 0; c < b->get_num_cols(); c++) {
    for (int r = 0; r < b->get_num_rows(); r++) {
      if (b->is_legal_move(c, r, symbol)) {
        auto child = new Move(c,r);
        children.push_back(child);
      }
    }
  }

  return children;
}

// TODO Fix heuristic
// TODO Fix symbol passing into minimax
Node* minimax(OthelloBoard* b, Move* m, bool maximizing_player, char symbol) {
  auto opponent_symbol = (symbol == 'X') ? 'O' : 'X';
  auto children = successor(b, symbol);

  if (children.empty()) {
    auto value = heuristic(b, symbol, opponent_symbol);
    return new Node(m, value); // ->(node)
  }

  // Maximizing Player
  if (maximizing_player) {
    auto best_value = new Node(NULL, -9999999); // (node) 
    std::for_each(children.begin(), children.end(), [b, &best_value, symbol, opponent_symbol](Move* m) mutable {
      auto clone = new OthelloBoard(*b);
      clone->play_move(m->col, m->row, symbol);

      auto v = minimax(clone, m, false, opponent_symbol); // <-(move,board) ->(move,score)
      best_value = (best_value->score > v->score) ? best_value : v; // <-(score, score)
    });
    return best_value; // ->(node)
  } 
 
  // Minimizing Player
  auto best_value = new Node(NULL, 9999999);
  std::for_each(children.begin(), children.end(), [b, &best_value, symbol, opponent_symbol](Move* m) mutable {
    auto clone = new OthelloBoard(*b);
    clone->play_move(m->col, m->row, symbol);

    auto v = minimax(b, m, true, opponent_symbol); 
    best_value = (best_value->score < v->score) ? best_value : v; 
  });
  return best_value;
}

MinimaxPlayer::MinimaxPlayer(char symb) : Player(symb) {
}

MinimaxPlayer::~MinimaxPlayer() {
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
  // Run minimax on tree
  auto result = minimax(b, NULL, true, this->symbol);

  result->print();
  exit(-1);

  // assign col and row
  col = result->move->col;
  row = result->move->row;
}

MinimaxPlayer* MinimaxPlayer::clone() {
  MinimaxPlayer* result = new MinimaxPlayer(symbol);
  return result;
}
