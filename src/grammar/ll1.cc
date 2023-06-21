//
// Created by Eric2i Hsiung on 2023/6/20.
//
#include "../include/grammar/ll1.h"

namespace grammar {

const std::string kEoi = "$";
const std::string kEps = "<EPS>";

void LL1Parser::eliminate_left_recursion() {
  auto check = [&]() {
    return this->symbol_table.size() == this->id2sym.size();
  };

  for (auto Ai = this->nonterminals.begin(); Ai != this->nonterminals.end(); ++Ai) {
    int i = *Ai;
    // std::cerr << "Processing: " << id2sym[i] << "\n";
    for (auto Aj = this->nonterminals.begin(); Aj != Ai; ++Aj) {
      int j = *Aj;
      /* replace Ai -> Aj \gamma with
          Ai -> \delta1 \gamma | \delta2 \gamma | ...
          if Aj -> \delta1 | \delta2 | ... are all Aj current productions
      */
      for (int k = 0; k < this->rules.size(); k++) {
        auto rule = this->rules[k];
        if (rule[0] == i && rule[1] == j) {
          // print_rule(rule);
          // std::cerr << "Before remove: " << this->rules.size() << "\n";
          for (auto &d : this->get_rules(j)) {
            std::vector<int> new_rule = {i};
            new_rule.insert(new_rule.end(), d.begin() + 1, d.end());
            new_rule.insert(new_rule.end(), rule.begin() + 2, rule.end());
            this->rules.push_back(new_rule);
            // std::cerr << "Adding one more rules!\n";
          }
          // remove this.rules[k]
          this->rules.erase(this->rules.begin() + k);
          // move k back one step
          k--;

          // std::cerr << "After update: " << this->rules.size() << "\n";
        }
      }
    }
    // eliminate the immediate left recursion among Ai-production
    std::vector<std::vector<int>> left_recursion;
    std::vector<std::vector<int>> non_left_recursion;
    for (auto rule_iter = this->rules.begin(); rule_iter != this->rules.end(); ++rule_iter) {
      std::vector<int> rule = *rule_iter;
      if (rule[0] == i && rule[1] == i) {
        // add rule to left_recurion
        left_recursion.push_back(rule);
        // remove this rule from this.rules
        this->rules.erase(rule_iter--);
      } else if (rule[0] == i) {
        // add rule to non_left_recursion
        non_left_recursion.push_back(rule);
        // remove this rule from this.rules
        this->rules.erase(rule_iter--);
      }
    }
    // if recursion exists
    if (left_recursion.size() > 0) {
      // std::cerr << "Processing recursion with head: " << id2sym[i] << "\n";
      // std::cerr << "with size: (" << left_recursion.size() << ", " << non_left_recursion.size() << ")\n";
      // edit left_recursion
      for (auto &rule : left_recursion) {
        // Ai -> Ai \alpha;
        rule.erase(rule.begin());
        rule.erase(rule.begin());
        // Ai' -> alpha Ai'
        std::string new_nonterminal = id2sym[i] + "'";
        if (symbol_table.find(new_nonterminal) == symbol_table.end())
          insert_nonterminal(new_nonterminal); // insert new nonterminal only when not exists
        rule.insert(rule.begin(), get_symbol_id(new_nonterminal));
        rule.push_back(get_symbol_id(new_nonterminal));
        assert(check());
        // print_rule(rule);
      }
      // edit non_left_recursion
      for (auto &rule : non_left_recursion) {
        rule.push_back(get_symbol_id(id2sym[i] + "'"));

        // print_rule(rule);
      }
      // add one new rules
      this->rules.push_back({get_symbol_id(id2sym[i] + "'"), get_symbol_id("<EPS>")});
    }
    // add all rules back
    this->rules.insert(this->rules.end(), left_recursion.begin(), left_recursion.end());
    this->rules.insert(this->rules.end(), non_left_recursion.begin(), non_left_recursion.end());

    // show();
  }
}

void LL1Parser::left_factoring() {
  for (auto i = nonterminals.rbegin(); i != nonterminals.rend(); ++i) {
    auto N = *i;
    // std::cerr << "scanning non-terminal: " << N << ", "<< id2sym[N] << '\n';
    std::vector<int> cur_rules_idx;
    for (int i = 0; i < this->rules.size(); i++) {
      if (this->rules[i][0] == N) {
        cur_rules_idx.push_back(i);
      }
    }
    if (cur_rules_idx.size() <= 1) {
      // there are less than two rules, no need to factor them
      continue;
    }

    std::map<int, std::vector<int>> prefixes; // prefix -> {rule_id1, rule_id2, ..., rule_idk}
    // group rules with same prefix(one char) together
    for (int i = 0; i < cur_rules_idx.size(); i++) {
      int cur_rule_id = cur_rules_idx[i];
      int next_symbol = this->rules[cur_rule_id][1];
      prefixes[next_symbol].push_back(cur_rule_id);
    }
    for (auto &prefix_pair : prefixes) {
      // prefix_pair: first -> prefix, second -> {rule_id1, rule_id2, ..., rule_idk}
      std::vector<int> prefix_rule_ids = prefix_pair.second;
      if (prefix_rule_ids.size() > 1) {
        // there are more than one rule sharing the same prefix
        insert_nonterminal(id2sym[N] + "\'");
        int new_nonterminal = get_symbol_id(id2sym[N] + "\'");
        this->rules.push_back({N, prefix_pair.first, new_nonterminal});
        // create new rules by removing the common prefix
        for (auto id : prefix_rule_ids) {
          std::vector<int> old_rule_rhs = this->rules[id];
          old_rule_rhs.erase(old_rule_rhs.begin(), old_rule_rhs.begin() + 2);
          if (old_rule_rhs.size() == 0) {
            std::vector<int> epsilon_rule = {new_nonterminal, symbol_table["<EPS>"]};
            this->rules[id] = epsilon_rule;
          } else {
            // create a new rule that appends the rest of the old rule to the new nonterminal
            old_rule_rhs.insert(old_rule_rhs.begin(), new_nonterminal);
            this->rules[id] = old_rule_rhs;
          }
        }
      }
    }
  }
}

void LL1Parser::build_first() {
  std::set<int> terminals;
  for (auto sym : symbol_table) {
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

void LL1Parser::build_follow() {
  // Initialize follow sets
  for (int id : nonterminals) {
    FOLLOW[id] = {};
  }
  FOLLOW[S].insert(get_symbol_id(kEoi)); // Follow of start symbol is $

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

void LL1Parser::build_parsing_table() {
  for (int r = 0; r < rules.size(); r++) {
    int nt = rules[r][0];
    for (int t : FIRST[rules[r][1]]) {
      if (t != get_symbol_id(kEps)) {
        parsing_table[{nt, t}] = r;
        // std::cerr << "inserting " << nt << id2sym[nt] << " " << t << " " << r << "\n";
      } else {
        for (int t : FOLLOW[nt]) {
          parsing_table[{nt, t}] = r;
          // std::cerr << "inserting " << nt << " " << t << " " << r << "\n";
        }
      }
    }
  }
}

bool LL1Parser::parse(std::vector<std::string> &input) {
  std::stack<int> stk;
  stk.push(get_symbol_id(kEoi));
  stk.push(S);

  input.push_back(kEoi);

  for (int i = 0; i < input.size(); i++) {
    // output stk
    // std::cerr << "Stack: ";
    // stack<int> tmp = stk;
    // while(!tmp.empty()) {
    //     std::cerr << id2sym[tmp.top()] << " ";
    //     tmp.pop();
    // }
    // std::cerr << "\n";
    // std::cerr << "input: ";
    // for(int j = i; j < input.size(); j++) std::cerr << input[j] << " ";
    // std::cerr << "\n";

    int symbol = stk.top();
    int id = get_symbol_id(input[i]);
    if (nonterminals.find(symbol) == nonterminals.end()) {
      // symbol is a terminal symbol
      if (symbol == id) {
        stk.pop();
      } else {
        std::cerr << "Error: \n\t" << input[i] << " is not expected at " << i << "\n";
        std::cerr << "\tSince we are expecting " << id2sym[symbol] << "\n";
        return false;
      }
    } else if (parsing_table.count({symbol, id}) == 0) {
      std::cerr << "Error: \n\t" << input[i] << " is not expected at " << i << "\n";
      std::cerr << "\tSince we have no prediction when " << input[i] << " follows " << id2sym[symbol] << "\n";
      // try error recovery
      bool foundReplacement = false;
      for (auto recovery : symbol_table) {
        // if recovery is not a nonterminal
        if (nonterminals.find(recovery.second) == nonterminals.end()) {
          if (parsing_table.count({symbol, recovery.second}) != 0) {
            std::cerr << "\tTry replace with " << input[i] << " with " << recovery.first << "\n";
            input[i] = recovery.first;
            --i; // reprocess the current input
            foundReplacement = true;
            break;
          }
        }
      }
      if (!foundReplacement)
        return false;
    } else {
      int prediction = parsing_table[{symbol, id}];

      stk.pop();
      // push rules[action]-rhs to stk in reversed order
      for (int j = rules[prediction].size() - 1; j > 0; j--) {
        if (get_symbol_id(kEps) != rules[prediction][j])
          // ignore epsilon
          stk.push(rules[prediction][j]);
      }
      --i;
    }
  }

  // std::cerr << "Input Buffer processing complete!" << '\n';
  if (stk.empty())
    return true;
  else
    return false;
}

}