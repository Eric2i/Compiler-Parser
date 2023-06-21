//
// Created by Eric2i Hsiung on 2023/6/20.
//

#ifndef PARSER_LR0_H
#define PARSER_LR0_H

#include "grammar.h"

namespace grammar {
class LR0Parser : public Grammar {
 protected:
  DFA dfa;

 public:
  LR0Parser(path rules) : Grammar(rules) {}; // constructor: read rules from file
  void augmentate();  // augment the grammar
  void build_dfa();   // build DFA
  bool show_dfa();    // output DFA
  bool lr0_dfa_items_check(std::set<item_t>); // check if multiple reducable items exist in one state
  bool parse(std::vector<std::string>); // DFA-driven parsing
  std::set<item_t> closure(std::set<item_t>); // items closure operation
  std::set<item_t> go(std::set<item_t>, int); // items transition operation
};
}
#endif //PARSER_LR0_H
