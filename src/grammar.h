#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>

struct Grammar {
    std::vector<std::vector<int>> rules;
    std::map<std::string, int> symbol_table;
    std::set<int> nonterminals;

    std::vector<std::vector<int>> read_grammar(std::string);
};

#endif