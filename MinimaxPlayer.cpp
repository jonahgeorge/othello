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

int heuristic(OthelloBoard* b) {
  return b->count_score('O') - b->count_score('X');
}

typedef std::tuple<OthelloBoard*, Move*> BoardMove; 

vector<BoardMove> successor(OthelloBoard* b, char symbol) {
  vector<BoardMove> children;

  for (int c = 0; c < b->get_num_cols(); c++) {
    for (int r = 0; r < b->get_num_rows(); r++) {
      if (b->is_legal_move(c, r, symbol)) {
        auto move = new Move(c,r);
        auto board = new OthelloBoard(*b);
        board->play_move(move->col, move->row, symbol);
        children.push_back(make_tuple(board, move));
      }
    }
  }

  return children;
}

void print_successors(vector<BoardMove> ts) {
  for_each(ts.begin(), ts.end(), [](BoardMove t) {
    get<0>(t)->display();
  });
}

// TODO Fix heuristic
// TODO Fix symbol passing into minimax
Node* minimax(OthelloBoard* b, bool maximizing_player, char symbol) {
  auto successors = successor(b, symbol);
  auto opponent_symbol = (symbol == 'X') ? 'O' : 'X';

  if (successors.empty()) {
    auto value = heuristic(b);
    return new Node(NULL, value); 
  }

  if (maximizing_player) {
    // Maximizing Player
    auto best_value = new Node(NULL, -999); 
    for_each(successors.begin(), successors.end(), 
        [b, &best_value, opponent_symbol](BoardMove t) mutable {
          auto v = minimax(get<0>(t), false, opponent_symbol); 
          // best_value = (v->score > best_value->score) ? v : best_value;
          best_value = (v->score > best_value->score) ? new Node(get<1>(t), v->score) : best_value;
        });
    return best_value; 
  } else {
    // Minimizing Player
    auto best_value = new Node(NULL, 999);
    for_each(successors.begin(), successors.end(), 
        [b, &best_value, opponent_symbol](BoardMove t) mutable {
          auto v = minimax(get<0>(t), true, opponent_symbol); 
          // best_value = (v->score < best_value->score) ? v : best_value;
          best_value = (v->score < best_value->score) ? new Node(get<1>(t), v->score) : best_value;
        });
    return best_value;
  }
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
  // Run minimax on tree
  auto result = minimax(b, true, this->symbol);

  result->print();

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
