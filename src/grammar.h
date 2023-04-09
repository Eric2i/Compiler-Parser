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
    std::map<int, std::string> id2sym;
    std::set<int> nonterminals; 
    std::map<int, std::set<int>> FIRST;

    void read_grammar(std::string);
    void eliminate_left_recursion();
    void left_factoring();
    void build_first();
    void build_follow();
    void build_parsing_table();

    std::vector<std::vector<int>> get_rules(int);
    std::vector<std::vector<int>> get_rules();
    int insert_nonterminal(const std::string &);
    int insert_terminal(const std::string &);
    int get_symbol_id(const std::string &);

    // DEBUG
    void show();
    void print_rule(std::vector<int>);
};

#endif