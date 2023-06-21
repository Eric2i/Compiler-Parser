//
// Created by Eric2i Hsiung on 2023/6/21.
//

#ifndef PARSER_SRC_INCLUDE_GRAMMAR_SLR1_H_
#define PARSER_SRC_INCLUDE_GRAMMAR_SLR1_H_

#endif //PARSER_SRC_INCLUDE_GRAMMAR_SLR1_H_

#include <utility>

#include "lr0.h"

namespace grammar {

class SLR1Parser : public LR0Parser {
 public:
  SLR1Parser(path rules_file_path); // constructor: read rules from file
  void build_first();                     // build FIRST set
  void build_follow();                    // build FOLLOW set
};
}