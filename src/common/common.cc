//
// Created by Eric2i Hsiung on 2023/6/20.
//

#include "../include/common/common.h"

namespace grammar {

bool item_t::operator==(const item_t &other) const
{
  return rule_id == other.rule_id && dot_pos == other.dot_pos;
}

bool item_t::operator<(const item_t &other) const
{
  if (rule_id != other.rule_id)
    return rule_id < other.rule_id;
  return dot_pos < other.dot_pos;
}

bool Item::reducable() const {
  return reducable_;
}

void Item::set_reducable(bool is_reducable) {
  reducable_ = is_reducable;
}

node_t DFANode::counter = 0;

DFANode::DFANode() {
  this->id = this->counter++;
}

node_t DFANode::get_id() {
  return this->id;
}

bool DFANode::isAccept() {
  return this->_isAccept;
}

bool DFANode::isStart() {
  return this->_isStart;
}

std::set<item_t> DFANode::get_items() {
  return this->items;
}

void DFANode::add_items(std::set<item_t> items) {
  for (auto item : items)
    this->items.insert(item);
}

void DFANode::add_next_node(symbol_t symbol, DFANode *node) {
  this->next[symbol] = node;
}

std::map<symbol_t, DFANode *> DFANode::get_next() {
  return this->next;
}

DFANode *DFANode::transit(symbol_t symbol) {
  return this->next[symbol];
}

void DFANode::show() {
  std::cerr << "[" << this->id << "]\n";
  std::cerr << "Items: \n";
  for (item_t item : this->items) {
    std::cerr << item.rule_id << ", " << item.dot_pos << std::endl;
  }
  std::cerr << "Edges: \n";
  for (auto symbolNodePair : this->next) {
    std::cerr << symbolNodePair.first << " -> " << symbolNodePair.second->get_id() << "\n";
  }
  std::cerr << std::endl;
}

DFANode *DFA::get_start() {
  return this->start;
}

void DFA::set_start(DFANode *start) {
  this->start = start;
}


}