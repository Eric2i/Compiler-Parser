#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <utility>

typedef std::vector<int> rule_t;

struct item_t {
    int rule_id;
    int dot_pos;

    item_t(int rule_id, int dot_pos): rule_id(rule_id), dot_pos(dot_pos) {}
    bool operator==(const item_t &) const;
    bool operator<(const item_t &) const;
};

class Grammar {
    private:
    int S;                                      // S
    std::vector<std::vector<int>> rules;        // P
    std::map<std::string, int> symbol_table;    // {N + T} -> {idx}
    std::map<int, std::string> id2sym;          // {idx} -> {N + T}
    std::set<int> nonterminals;                 // N
    std::map<int, std::set<int>> FIRST;         // FIRST set
    std::map<int, std::set<int>> FOLLOW;        // FOLLOW set
    std::map<std::pair<int, int>, int> parsing_table; // parsing table
    std::vector<std::set<item_t>> states;

    public:
    Grammar(std::string);                       // Initialization
    void eliminate_left_recursion();            // eliminate left recursion
    void left_factoring();                      // eliminate left factors
    void build_first();                          // build FIRST set 
    void build_follow();                        // build FOLLOW set
    void build_parsing_table();                 // build parsing table
    bool parse(std::vector<std::string>&);      // Table-driven parsing

    void augmentate();
    void build_states();
    void build_goto();
    void build_action();

    std::vector<std::vector<int>> get_rules(int);
    std::vector<std::vector<int>> get_rules();
    int insert_nonterminal(const std::string &);
    int insert_terminal(const std::string &);
    int get_symbol_id(const std::string &);
    std::set<item_t> closure(std::set<item_t>);
    std::set<item_t> go(std::set<item_t>, int);

    // DEBUG
    void show();
    void print_rule(std::vector<int>);
    void print_item(const item_t &);
};

#endif