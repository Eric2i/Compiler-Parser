//
// Created by Eric2i Hsiung on 2023/6/21.
//

#include "../include/grammar/slr1.h"

namespace grammar {

const std::string kEoi = "$";
const std::string kEps = "<EPS>";

void SLR1Parser::build_first() {
  std::set<int> terminals;
  for (const auto& sym : symbol_table) {
    int id = sym.second;
    if (nonterminals.find(id) == nonterminals.end()) {
      terminals.insert(id);
    }
  }

  // Initialize FIRST sets for terminals
  for (auto t : terminals) {
    FIRST[t].insert(t);
  }

  // Initialize FIRST sets for nonterminals
  for (auto nt : nonterminals) {
    FIRST[nt] = std::set<int>();
  }

  bool changed = true;
  while (changed) {
    changed = false;
    // Iterate over all the rules
    for (auto r : rules) {
      int lhs = r[0];
      bool exists_eps = true;
      // Compute FIRST set for the RHS of the rule
      for (size_t i = 1; i < r.size(); i++) {
        // If the symbol is a terminal, add it to FIRST set and break
        if (terminals.count(r[i]) > 0) {
          if (FIRST[lhs].count(r[i]) == 0) {
            changed = true;
          }
          FIRST[lhs].insert(r[i]);
          exists_eps = false;
          break;
        }
        // If the symbol is a nonterminal
        // add its FIRST set except {EPS} to FIRST set of the head symbol
        for (auto f : FIRST[r[i]]) {
          if (f == get_symbol_id("<EPS>")) {
            continue;
          }
          if (FIRST[lhs].count(f) == 0) {
            changed = true;
          }
          FIRST[lhs].insert(f);
        }
        // If FIRST set of the nonterminal does not contain epsilon, break
        if (FIRST[r[i]].count(get_symbol_id("<EPS>")) == 0) {
          exists_eps = false;
          break;
        }
      }
      // If RHS can derive epsilon, add epsilon to FIRST set of head symbol
      if (exists_eps) {
        if (FIRST[lhs].count(get_symbol_id("<EPS>")) == 0) {
          changed = true;
        }
        FIRST[lhs].insert(get_symbol_id("<EPS>"));
      }
    }
  }
}

void SLR1Parser::build_follow() {
  // Initialize follow sets
  for (int id : nonterminals) {
    FOLLOW[id] = {};
  }
  FOLLOW[S].insert(get_symbol_id(kEoi)); // Follow of start symbol is $
  std::cerr << "S:" << id2sym[S] << "has follow:"; for(auto f: FOLLOW[-2]) std::cerr << id2sym[f] << " "; std::cerr << std::endl;

  // Iterate until all follow sets are stable(unchanged)
  bool changed = true;
  while (changed) {
    changed = false;
    // Iterate over all rules
    for (auto &rule : rules) {
      int A = rule[0];
      for (int i = 1; i < rule.size(); i++) {
        int B = rule[i];
        if (nonterminals.count(B)) {
          // Add first(beta) to follow(B) for each beta in C1 C2 ... Ck
          int j = i + 1;
          bool epsilon = true;
          while (j < rule.size() && epsilon) {
            int C = rule[j];
            for (int c : FIRST[C]) {
              if (c != get_symbol_id(kEps)) {
                if (FOLLOW[B].insert(c).second) {
                  changed = true;
                }
              } else {
                epsilon = true;
              }
            }
            if (FIRST[C].count(get_symbol_id(kEps))) {
              j++;
            } else {
              epsilon = false;
            }
          }
          // If epsilon in first(beta) for all beta in C1 C2 ... Ck
          if (epsilon) {
            for (int c : FOLLOW[A]) {
              if (FOLLOW[B].insert(c).second) {
                changed = true;
              }
            }
          }
        }
      }
    }
  }
}

SLR1Parser::SLR1Parser(path rules_file_path) : LR0Parser(rules_file_path) {}

}