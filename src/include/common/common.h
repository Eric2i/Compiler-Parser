//
// Created by Eric2i Hsiung on 2023/6/20.
//
// Here we declare:
// i)  item, which consists of a production, a dot position and a lookahead symbol,
// ii) DFANode and DFA, which describe the LR(0) or LR(1) automaton.

#ifndef PARSER_SRC_INCLUDE_COMMON_COMMON_H_
#define PARSER_SRC_INCLUDE_COMMON_COMMON_H_

#include <vector>
#include <map>
#include <set>
#include <string>
#include <stack>
#include <algorithm>
#include <utility>
#include <iostream>
#include <spdlog/spdlog.h>

namespace grammar {
using path = std::string;
using symbol_t = std::string;

struct Item {
  int rule_id;
  int dot_pos;
  bool reducable_;

  bool reducable() const;
  void set_reducable(bool);

  Item(int rule_id, int dot_pos) : rule_id(rule_id), dot_pos(dot_pos) {}
  bool operator==(const Item &) const;
  bool operator<(const Item &) const;
};
using item_t = Item;

using node_t = long long;
class DFANode {
 private:
  node_t id;
  static node_t counter;
  bool _isAccept;
  bool _isStart;
  std::set<item_t> items;
  std::map<symbol_t, DFANode *> next;

 public:
  DFANode();
  node_t get_id();
  bool isAccept();
  bool isStart();
  std::set<item_t>& get_items();
  void add_items(std::set<item_t>);
  void add_next_node(symbol_t, DFANode *);
  std::map<symbol_t, DFANode *> get_next();
  DFANode *transit(symbol_t);
  void show();
};
class DFA {
 private:
  DFANode *start;

 public:
  DFA() = default;
  DFANode *get_start();
  void set_start(DFANode *);
};
}

#endif //PARSER_SRC_INCLUDE_COMMON_COMMON_H_
