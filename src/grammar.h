#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <utility>

namespace grammar
{
    using rule_t = std::vector<int>;

    struct item_t
    {
        int rule_id;
        int dot_pos;

        item_t(int rule_id, int dot_pos) : rule_id(rule_id), dot_pos(dot_pos) {}
        bool operator==(const item_t &) const;
        bool operator<(const item_t &) const;
    };

    using path = std::string;
    using symbol_t = std::string;

    class Grammar
    {
    protected:
        int S;                                            // S
        std::vector<std::vector<int>> rules;              // P
        std::map<symbol_t, int> symbol_table;             // {N + T} -> {idx}
        std::map<int, std::string> id2sym;                // {idx} -> {N + T}
        std::set<int> nonterminals;                       // N
        std::map<int, std::set<int>> FIRST;               // FIRST set
        std::map<int, std::set<int>> FOLLOW;              // FOLLOW set
        std::map<std::pair<int, int>, int> parsing_table; // parsing table
        std::vector<std::set<item_t>> states;

    public:
        Grammar(path); // Initialization
        ~Grammar() = default;

        std::vector<std::vector<int>> get_rules(int);
        std::vector<std::vector<int>> get_rules();
        int insert_nonterminal(const std::string &);
        int insert_terminal(const std::string &);
        int get_symbol_id(const std::string &);
        bool inSymbolTable(symbol_t);

        // DEBUG
        void show();
        void print_rule(std::vector<int>);
        void print_item(const item_t &);
    };

    class LL1Parser : public Grammar
    {
    public:
        LL1Parser(path rules) : Grammar(rules){};
        void eliminate_left_recursion();        // eliminate left recursion
        void left_factoring();                  // eliminate left factors
        void build_first();                     // build FIRST set
        void build_follow();                    // build FOLLOW set
        void build_parsing_table();             // build parsing table
        bool parse(std::vector<std::string> &); // Table-driven parsing
    };

    using label_t = std::string;
    using node_t = long long;

    class DFANode
    {
    private:
        node_t id;
        static node_t counter;
        bool _isAccept;
        bool _isStart;
        std::set<item_t> items;
        std::map<symbol_t, DFANode *> next;

    public:
        DFANode();
        node_t get_id();
        bool isAccept();
        bool isStart();
        std::set<item_t> get_items();
        void add_items(std::set<item_t>);
        void add_next_node(symbol_t, DFANode *);
        std::map<symbol_t, DFANode *> get_next();
        DFANode *transit(symbol_t);
        void show();
    };

    class DFA
    {
    private:
        DFANode *start;

    public:
        DFA() = default;
        DFANode *get_start();
        void set_start(DFANode *);
    };

    class LR0Parser : public Grammar
    {
    protected:
        DFA dfa;

    public:
        LR0Parser(path rules) : Grammar(rules){};
        void augmentate();
        void build_dfa();
        bool show_dfa();
        bool lr0_dfa_items_check(std::set<item_t>);
        bool parse(std::vector<std::string>);
        std::set<item_t> closure(std::set<item_t>);
        std::set<item_t> go(std::set<item_t>, int);
    };
}

#endif