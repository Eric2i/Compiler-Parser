//
// Created by Eric2i Hsiung on 2023/6/20.
//

#ifndef PARSER_LL1_H
#define PARSER_LL1_H

#include "grammar.h"

namespace grammar {
class LL1Parser : public Grammar {
 public:
  LL1Parser(path rules) : Grammar(rules) {}; // constructor: read rules from file
  void eliminate_left_recursion();        // eliminate left recursion
  void left_factoring();                  // eliminate left factors
  void build_first();                     // build FIRST set
  void build_follow();                    // build FOLLOW set
  void build_parsing_table();             // build parsing table
  bool parse(std::vector<std::string> &); // Table-driven parsing
};
}
#endif //PARSER_LL1_H
