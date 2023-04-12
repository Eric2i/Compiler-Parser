#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <utility>

class Grammar {
    private:
    int S;                                      // S
    std::vector<std::vector<int>> rules;        // P
    std::map<std::string, int> symbol_table;    // N + T
    std::map<int, std::string> id2sym;          // {N + T} -> {idx}
    std::set<int> nonterminals;                 // N
    std::map<int, std::set<int>> FIRST;         // FIRST set
    std::map<int, std::set<int>> FOLLOW;        // FOLLOW set
    std::map<std::pair<int, int>, int> parsing_table; // parsing table

    public:
    Grammar(std::string);                       // Initialization
    void eliminate_left_recursion();            // eliminate left recursion
    void left_factoring();                      // eliminate left factors
    void build_first();                          // build FIRST set 
    void build_follow();                        // build FOLLOW set
    void build_parsing_table();                 // build parsing table

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