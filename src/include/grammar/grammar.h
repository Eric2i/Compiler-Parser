#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "../common/common.h"
#include <fstream>
#include <sstream>
#include <cassert>

namespace grammar {
class Grammar {
 protected:
  int S;                                            // S
  std::vector<std::vector<int>> rules;              // P
  std::map<symbol_t, int> symbol_table;             // {N + T} -> {idx}
  std::map<int, std::string> id2sym;                // {idx} -> {N + T}
  std::set<int> nonterminals;                       // N
  std::map<int, std::set<int>> FIRST;               // FIRST set
  std::map<int, std::set<int>> FOLLOW;              // FOLLOW set
  std::map<std::pair<int, int>, int> parsing_table; // parsing table
  std::vector<std::set<item_t>> states;

 public:
  Grammar(path); // Initialization
  ~Grammar() = default;

  std::vector<std::vector<int>> get_rules(int);
  std::vector<std::vector<int>> get_rules();
  int insert_nonterminal(const std::string &);
  int insert_terminal(const std::string &);
  int get_symbol_id(const std::string &);
  bool inSymbolTable(symbol_t);

  // DEBUG
  void show();
  void print_rule(std::vector<int>);
  void print_item(const item_t &);
};

}

#endif