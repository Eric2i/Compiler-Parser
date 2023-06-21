//
// Created by Eric2i Hsiung on 2023/6/20.
//

#include "../include/grammar/lr0.h"

namespace grammar {

void LR0Parser::augmentate() {
  insert_nonterminal("<S>");
  this->S = get_symbol_id("<S>"); // update S as the start symbol of grammar
  rules.push_back({get_symbol_id("<S>"), *(nonterminals.rbegin())});
}

std::set<item_t> LR0Parser::closure(std::set<item_t> items) {
  // prepare a stack to store all item in items
  std::stack<item_t> stk;
  for (item_t item : items) {
    stk.push(item);
  }
  // implement item closure operation
  std::set<item_t> closure;
  while (!stk.empty()) {
    item_t item = stk.top();
    stk.pop();

    // std::cerr << "Having Item: "; print_item(item);

    closure.insert(item);
    int dot = item.dot_pos;
    if (dot == rules[item.rule_id].size()) {
      // dot exists at the end of the production -> reduce item
      // std::cerr << "dot exists at the end of the production!\n";
      closure.insert(item);
    } else if (dot < rules[item.rule_id].size()) {
      // dot exists in the middle of the production -> basic item / initial item
      int next_symbol = rules[item.rule_id][dot];
      if (nonterminals.find(next_symbol) != nonterminals.end()) {
        // nonterminal after the dot
        for (int r = 0; r < rules.size(); r++) {
          if (rules[r][0] == next_symbol) {
            // generate new item
            item_t new_item(r, 1);
            if (closure.find(new_item) == closure.end())
              stk.push(new_item);
          }
        }
        // std::cerr << "Find nonterminal after the dot\n";
        closure.insert(item);
      } else {
        // terminal after the dot
        // std::cerr << "Find terminal after the dot\n";
        closure.insert(item);
      }
    }
  }
  // std::cerr << "================================================================================\n";
  // for(auto &item: closure) {
  //     print_item(item);
  // }
  // std::cerr << "================================================================================\n";
  return closure;
}

std::set<item_t> LR0Parser::go(std::set<item_t> I, int X) {
  std::set<item_t> J;
  for (item_t item : I) {
    if (item.dot_pos < this->rules[item.rule_id].size() && this->rules[item.rule_id][item.dot_pos] == X) {
      J.insert(item_t(item.rule_id, item.dot_pos + 1));
    }
  }
  J = this->closure(J);

  // std::cerr << "================================================================================\n";
  // for (auto each : J)
  // {
  //     print_item(each);
  // }
  // std::cerr << "================================================================================\n";

  return J;
}

void LR0Parser::build_dfa() {
  DFANode *start = new DFANode;
  start->add_items(closure(std::set<item_t>({item_t(this->rules.size() - 1, 1)})));
  this->dfa.set_start(start);

  std::stack<DFANode *> dfa_node_ptr_stk;
  std::map<std::set<item_t>, DFANode *> items_to_dfanode;
  dfa_node_ptr_stk.push(start);
  while (!dfa_node_ptr_stk.empty()) {
    DFANode *cur_node = dfa_node_ptr_stk.top();
    dfa_node_ptr_stk.pop();
    std::set<item_t> cur_items = cur_node->get_items();

    for (auto symbol2id : symbol_table) {
      symbol_t symbol = symbol2id.first;
      int symbol_id = symbol2id.second;

      std::set<item_t> next_items = closure(go(cur_items, symbol_id));
      if (lr0_dfa_items_check(next_items) == false) {
        std::cerr << "Grammar rules Error: LR0 check failed!\n";
        exit(-1);
      }
      if (next_items.empty())
        continue;

      if (items_to_dfanode.find(next_items) == items_to_dfanode.end()) {
        DFANode *next_node = new DFANode;

        next_node->add_items(next_items);
        items_to_dfanode[next_items] = next_node;

        cur_node->add_next_node(this->id2sym[symbol_id], next_node);
        dfa_node_ptr_stk.push(next_node);
      } else {
        cur_node->add_next_node(this->id2sym[symbol_id], items_to_dfanode[next_items]);
      }
    }
  }
}

bool LR0Parser::show_dfa() {
  std::cerr << "\nDFA:\n";
  std::cerr << "================================================================================\n";
  std::stack<DFANode *> dfa_node_ptr_stk;
  std::set<DFANode *> visited;
  dfa_node_ptr_stk.push(this->dfa.get_start());
  while (!dfa_node_ptr_stk.empty()) {
    DFANode *cur_node = dfa_node_ptr_stk.top();
    dfa_node_ptr_stk.pop();
    visited.insert(cur_node);
    std::cerr << "Node [" << cur_node->get_id() << "]:\n";
    std::cerr << "++++++++++\n";
    for (auto item : cur_node->get_items()) {
      print_item(item);
    }

    if (cur_node->get_next().size())std::cerr << "Out Edges:\n";
    for (auto symbolNodePair : cur_node->get_next()) {
      std::cerr << "[" << cur_node->get_id() << "]"
                << "-" << symbolNodePair.first << "->[" << symbolNodePair.second->get_id() << "]\n";
      if (visited.find(symbolNodePair.second) == visited.end())
        dfa_node_ptr_stk.push(symbolNodePair.second);
    }
    std::cerr << "++++++++++\n\n";
  }
  std::cerr << "================================================================================\n";
  return true;
}

bool LR0Parser::lr0_dfa_items_check(std::set<item_t> items) {
  // if multiple reducable item exists, return false
  bool has_reducable_item = false;
  for (auto item : items) {
    if (item.dot_pos == this->rules[item.rule_id].size()) {
      if (has_reducable_item) {
        return false;
      }
      has_reducable_item = true;
    }
  }
  return true;
}

bool LR0Parser::parse(std::vector<std::string> symbols) {
  DFANode *cur_node = this->dfa.get_start();
  std::stack<DFANode *> dfa_node_ptr_stk;
  dfa_node_ptr_stk.push(cur_node);

  int i = 0;
  while (i < symbols.size() && !dfa_node_ptr_stk.empty()) {
    auto symbol = symbols[i];
    if (!inSymbolTable(symbol)) {
      std::cerr << "Unknown Symbol Detected: " << symbol << "\n";
      return false;
    }
    cur_node = dfa_node_ptr_stk.top();

    if (cur_node->transit(symbol) != nullptr) {
      // shift operation
      dfa_node_ptr_stk.push(cur_node->transit(symbol));
      i++;
    } else {
      // reduce operation
      item_t item = *(cur_node->get_items().begin());

      int reduce_rule_id = item.rule_id;
      int reduce_rule_len = this->rules[reduce_rule_id].size() - 1; // length of RHS
      if (dfa_node_ptr_stk.size() < reduce_rule_len + 1) {
        std::cerr << "Syntax Error: Unexpected Symbol: " << symbol << "\n";
        return false;
      }
      for (int j = 0; j < reduce_rule_len; j++) {
        dfa_node_ptr_stk.pop();
      }

      cur_node = dfa_node_ptr_stk.top();
      dfa_node_ptr_stk.push(cur_node->transit(this->id2sym[this->rules[reduce_rule_id][0]]));
    }
  }

  while (!dfa_node_ptr_stk.empty()) {
    item_t item = *(dfa_node_ptr_stk.top()->get_items().begin());

    int reduce_rule_id = item.rule_id;
    int reduce_rule_len = this->rules[reduce_rule_id].size() - 1; // length of RHS
    for (int j = 0; j < reduce_rule_len; j++) {
      dfa_node_ptr_stk.pop();
    }

    cur_node = dfa_node_ptr_stk.top();

    if (cur_node->get_id() == 0 && this->id2sym[this->rules[reduce_rule_id][0]] == "<S>")
      return true;

    dfa_node_ptr_stk.push(cur_node->transit(this->id2sym[this->rules[reduce_rule_id][0]]));
  }

  return false;
}

}