#include "grammar.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const string EOI = "$";
const string EPS = "<EPS>";

int Grammar::insert_nonterminal(const string &s) {
    // register new symbol, with id <= 0
    int id = -(this->symbol_table.size());
    this->symbol_table[s] = id;
    this->id2sym[id] = s;
    this->nonterminals.insert(id);
    return id;
}

int Grammar::insert_terminal(const string &s) {
    // register new symbol, with id > 0
    int id = this->symbol_table.size();
    this->symbol_table[s] = id;
    this->id2sym[id] = s;
    return id;
}

// for non-terminal symbol only
int Grammar::get_symbol_id(const string &s) {
    // cerr << "looking up symol:" << s;
    auto iter = this->symbol_table.find(s);
    if (iter == this->symbol_table.end()) {
        return insert_terminal(s);
    } else {
        return iter->second;
    }
}

Grammar::Grammar(string filename) {
    ifstream file(filename);
    
    insert_terminal("<EPS>"); // register <EPS> as a special terminal symbol
    insert_terminal("$"); // register $ as end of input symbol

    string line;
    while(getline(file, line)) {
        vector<int> rule; 

        istringstream iss(line);
        string head, body, arrow;
        iss >> head >> arrow;

        nonterminals.insert(get_symbol_id(head));
        rule.push_back(get_symbol_id(head));
        
        string symbol;
        while(iss >> symbol) {
            if(symbol != "|") rule.push_back(get_symbol_id(symbol));
            else {
                this->rules.push_back(rule); // break the production into two rules
                rule = {get_symbol_id(head)};
            }
        }
        this->rules.push_back(rule);
    }
    
    // turn all nonterminals into negative numbers
    // update rules directly
    for(auto &rule : this->rules) {
        for(auto &i : rule) {
            if(nonterminals.find(i) != nonterminals.end()) {
                i = -i;
            }
        }
    }
    // update nonterminal id 
    set<int> new_nonterminals;
    for(auto i: nonterminals) {
        // cerr << "updating " << id2sym[i] << " " << i << "\n";
        symbol_table[id2sym[i]] = -i;
        id2sym[-i] = id2sym[i]; if(i != -i) id2sym.erase(i);
        new_nonterminals.insert(-i);
        // cerr << "to " << id2sym[-i] << " " << -i << "\n";
    }
    this->nonterminals = new_nonterminals;
    // specify <S>
    this->S = *nonterminals.rbegin();
}

vector<vector<int>> Grammar::get_rules(int head) {
    vector<vector<int>> rules;
    for(auto &rule: this->rules) {
        if(rule[0] == head) {
            rules.push_back(rule);
        }
    }
    return rules;
}

vector<vector<int>> Grammar::get_rules() {
    return this->rules;
}

void Grammar::eliminate_left_recursion() {
    for(auto Ai = this->nonterminals.begin(); Ai != this->nonterminals.end(); ++Ai) {
        int i = *Ai;
        // cerr << "Processing :" << id2sym[i] << "\n";
        for(auto Aj = this->nonterminals.begin(); Aj != Ai; ++Aj) {
            int j = *Aj;
            /* replace Ai -> Aj \gamma with
                Ai -> \delta1 \gamma | \delta2 \gamma | ...
                if Aj -> \delta1 | \delta2 | ... are all Aj current productions
            */
            for(int k = 0; k < this->rules.size(); k++) {
                auto rule = this->rules[k];
                if(rule[0] == i && rule[1] == j) {
                    // print_rule(rule);
                    // cerr << "Before remove: " << this->rules.size() << "\n";
                    for(auto &d: this->get_rules(j)) {
                        vector<int> new_rule = {i};
                        new_rule.insert(new_rule.end(), d.begin() + 1, d.end());
                        new_rule.insert(new_rule.end(), rule.begin() + 2, rule.end());
                        this->rules.push_back(new_rule);
                        // cerr << "Adding one more rules!\n";
                    }
                    // remove this.rules[k]
                    this->rules.erase(this->rules.begin() + k);
                    // move k back one step
                    k--;

                    // cerr << "After update: " << this->rules.size() << "\n";
                }
            }
        }
        // eliminate the immediate left recursion among Ai-production
        vector<vector<int>> left_recursion;
        vector<vector<int>> non_left_recursion;
        for(auto rule_iter = this->rules.begin(); rule_iter != this->rules.end(); ++rule_iter) {
            vector<int> rule = *rule_iter;
            if(rule[0] == i && rule[1] == i) {
                // add rule to left_recurion
                left_recursion.push_back(rule);
                // remove this rule from this.rules
                this->rules.erase(rule_iter--);
            }
            else if(rule[0] == i) {
                // add rule to non_left_recursion
                non_left_recursion.push_back(rule);
                // remove this rule from this.rules
                this->rules.erase(rule_iter--);
            }
        }
        // if recursion exists
        if(left_recursion.size() > 0) {
            // cerr << "Processing recursion with head: " << id2sym[i] << "\n";
            // cerr << "with size: (" << left_recursion.size() << ", " << non_left_recursion.size() << ")\n";
            // edit left_recursion
            for(auto &rule: left_recursion) {
                // Ai -> Ai \alpha;
                rule.erase(rule.begin());
                rule.erase(rule.begin());
                // Ai' -> alpha Ai'
                insert_nonterminal(id2sym[i] + "'");
                rule.insert(rule.begin(), get_symbol_id(id2sym[i] + "'"));
                rule.push_back(get_symbol_id(id2sym[i] + "'"));
                // print_rule(rule);
            }
            // edit non_left_recursion
            for(auto &rule: non_left_recursion) {
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

void Grammar::left_factoring() {
    for(auto i =  nonterminals.rbegin(); i != nonterminals.rend(); ++i) {
        auto N = *i;
        // cerr << "scanning non-terminal: " << N << ", "<< id2sym[N] << '\n';
        vector<int> cur_rules_idx;
        for(int i = 0; i < this->rules.size(); i++) {
            if(this->rules[i][0] == N) {
                cur_rules_idx.push_back(i);
            }
        }
        if(cur_rules_idx.size() <= 1) {
            // there are less than two rules, no need to factor them
            continue;
        }

        map<int, vector<int>> prefixes; // prefix -> {rule_id1, rule_id2, ..., rule_idk}
        // group rules with same prefix(one char) together
        for(int i = 0; i < cur_rules_idx.size(); i++) {
            int cur_rule_id = cur_rules_idx[i];
            int next_symbol = this->rules[cur_rule_id][1];
            prefixes[next_symbol].push_back(cur_rule_id);
        }
        for(auto & prefix_pair: prefixes) {
            // prefix_pair: first -> prefix, second -> {rule_id1, rule_id2, ..., rule_idk}
            vector<int> prefix_rule_ids = prefix_pair.second;
            if(prefix_rule_ids.size() > 1) {
                // there are more than one rule sharing the same prefix
                insert_nonterminal(id2sym[N] + "\'");
                int new_nonterminal = get_symbol_id(id2sym[N] + "\'");
                this->rules.push_back({N, prefix_pair.first, new_nonterminal});
                // create new rules by removing the common prefix
                for(auto id: prefix_rule_ids) {
                    vector<int> old_rule_rhs = this->rules[id];
                    old_rule_rhs.erase(old_rule_rhs.begin(), old_rule_rhs.begin() + 2);
                    if(old_rule_rhs.size() == 0) {
                        vector<int> epsilon_rule = {new_nonterminal, symbol_table["<EPS>"]};
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

void Grammar::build_first() {
    set<int> terminals;
    for(auto sym: symbol_table) {
        int id = sym.second;
        if(nonterminals.find(id) == nonterminals.end()) {
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

void Grammar::build_follow() {
    // Initialize follow sets
    for (int id : nonterminals) {
        FOLLOW[id] = {};
    }
    FOLLOW[S].insert(get_symbol_id(EOI)); // Follow of start symbol is $

    // Iterate until all follow sets are stable(unchanged)
    bool changed = true;
    while (changed) {
        changed = false;
        // Iterate over all rules
        for (auto& rule : rules) {
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
                            if (c != get_symbol_id(EPS)) {
                                if (FOLLOW[B].insert(c).second) {
                                    changed = true;
                                }
                            } else {
                                epsilon = true;
                            }
                        }
                        if (FIRST[C].count(get_symbol_id(EPS))) {
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

void Grammar::build_parsing_table() {
    for(int r = 0; r < rules.size(); r++) {
        int nt = rules[r][0];
        for(int t: FIRST[rules[r][1]]) {
            if(t != get_symbol_id(EPS)) {
                parsing_table[{nt, t}] = r;
            } else {
                for(int t: FOLLOW[nt]) {
                    parsing_table[{nt, t}] = r;
                }
            }
        }
    }
}

// DEBUG
void Grammar::show() {
    // output a horizontal line using =
    auto print_line = []() {
        for(int i = 0; i < 80; i++) cerr << "=";
        cerr << "\n";
    };
    auto print_section = []() {
        for(int i = 0; i < 40; i++) cerr << "+";
        cerr << "\n";
    };

    print_line();
    // output symbol_table
    cerr << "SYMBOL:\n";
    for(auto &p: this->symbol_table) {
        // p: first -> string, second -> id
        if(nonterminals.find(p.second) != nonterminals.end() || p.second == 0)
            cerr << "(Non)" << p.first << ": " << p.second << "\n";
        else 
            cerr << "(Ter)" << p.first << ": " << p.second << "\n";
    }print_section();

    // output rules with id replaced by symbols using id2sym
    cerr << "RULES:\n";
    for(auto &rule: this->rules) {
        bool isFirst = true;
        for(auto &i: rule) {

            cerr << id2sym[i] 
            // << "[" << i << "]"
            ;

            if(isFirst) cout << "->", isFirst = false;
        }
        cerr << "\n";
    }print_section();

    cerr << "FIRST:\n";
    for(auto &p: this->FIRST) {
        cerr << id2sym[p.first] << ": "
        // << "[" << p.first << "]: "
        ;
        for(auto &i: p.second) {
            cerr << id2sym[i]
            // << "[" << i << "]"
            ;
        }
        cerr << "\n";
    }print_section();

    cerr << "FOLLOW:\n";
    for(auto &p: this->FOLLOW) {
        cerr << id2sym[p.first] << ": "
        // << "[" << p.first << "]: "
        ;
        for(auto &i: p.second) {
            cerr << id2sym[i]
            // << "[" << i << "]"
            ;
        }
        cerr << "\n";
    }print_section();

    cerr << "Parsing Table:\n";
    for(auto &p: this->parsing_table) {
        cerr << "(" << id2sym[p.first.first] << "," 
        << id2sym[p.first.second] << ") -> "
        << p.second << ": ";
        print_rule(rules[p.second]);
    }

    print_line();
}

// DEBUG
void Grammar::print_rule(vector<int> r) {
    // print the r with id replaced by symbols using id2sym
    bool isFirst = true;
    for(auto &i: r) {
        cerr << id2sym[i];
        if(isFirst) cout << "->", isFirst = false;
        // else cout << " ";
    }
    cout << endl;
}