#include "grammar.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int Grammar::get_symbol_id(const string &s) {
    // cerr << "looking up " << s;
    auto iter = this->symbol_table.find(s);
    if (iter == this->symbol_table.end()) {
        // register new symbol
        int id = this->symbol_table.size();
        if(s.back() == '\'') id = -id; // automatically turn id to -id if new nonter is added
        this->symbol_table[s] = id;
        this->id2sym[id] = s;
        // cerr << "with returned " << id << "\n";
        return id;
    } else {
        // cerr << "with returned " << iter->second << "\n";
        return iter->second;
    }
}

void Grammar::read_grammar(string filename) {
    ifstream file(filename);
    
    get_symbol_id("<S>"); // register <S> as the start symbol

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
    // update symbol_table and id2sym
    for(auto &i: nonterminals) {
        symbol_table[id2sym[i]] = -i;
        id2sym[-i] = id2sym[i];
        id2sym.erase(i);
    }
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
        for(auto Aj = this->nonterminals.begin(); Aj != Ai; ++Aj) {
            int j = *Aj;
            /* replace Ai -> Aj \gamma with
                Ai -> \delta1 \gamma | \delta2 \gamma | ...
                if Aj -> \delta1 | \delta2 | ... are all Aj current productions
            */
            for(int k = 0; k < this->rules.size(); k++) {
                auto rule = this->rules[k];
                if(rule[0] == -i && rule[1] == -j) {
                    // cerr << "Before remove: " << this->rules.size() << "\n";
                    for(auto &d: this->get_rules(-j)) {
                        vector<int> new_rule = {-i};
                        new_rule.insert(new_rule.end(), d.begin() + 1, d.end());
                        new_rule.insert(new_rule.end(), rule.begin() + 2, rule.end());
                        this->rules.push_back(new_rule);
                        // cerr << "Adding one more rules!\n";
                    }
                    // remove this.rules[k]
                    this->rules.erase(this->rules.begin() + k);
                    // cerr << "After update: " << this->rules.size() << "\n";
                }
            }
        }
        // show();
        // eliminate the immediate left recursion among Ai-production
        vector<vector<int>> left_recursion;
        vector<vector<int>> non_left_recursion;
        for(auto rule_iter = this->rules.begin(); rule_iter != this->rules.end(); ++rule_iter) {
            vector<int> rule = *rule_iter;
            if(rule[0] == -i && rule[1] == -i) {
                // add rule to left_recurion
                left_recursion.push_back(rule);
                // remove this rule from this.rules
                this->rules.erase(rule_iter--);
            }
            else if(rule[0] == -i) {
                // add rule to non_left_recursion
                non_left_recursion.push_back(rule);
                // remove this rule from this.rules
                this->rules.erase(rule_iter--);
            }
        }
        // if recursion exists
        if(left_recursion.size() > 0) {
            // cerr << "Processing recursion with head: " << id2sym[-i] << "\n";
            // cerr << "with size: (" << left_recursion.size() << ", " << non_left_recursion.size() << ")\n";
            // edit left_recursion
            for(auto &rule: left_recursion) {
                // Ai -> Ai \alpha;
                rule.erase(rule.begin());
                rule.erase(rule.begin());
                // Ai' -> alpha Ai'
                rule.insert(rule.begin(), get_symbol_id(id2sym[-i] + "'"));
                rule.push_back(get_symbol_id(id2sym[-i] + "'"));

                // print_rule(rule);
            }
            // edit non_left_recursion
            for(auto &rule: non_left_recursion) {
                rule.push_back(get_symbol_id(id2sym[-i] + "'"));
                
                // print_rule(rule);
            }
            // add one new rules
            this->rules.push_back({get_symbol_id(id2sym[-i] + "'"), get_symbol_id("<EPS>")});
        }
        // add all rules back
        this->rules.insert(this->rules.end(), left_recursion.begin(), left_recursion.end());
        this->rules.insert(this->rules.end(), non_left_recursion.begin(), non_left_recursion.end());
        
        show();
    }
} 

// DEBUG
void Grammar::show() {
    // output a horizontal line using =
    auto print_line = []() {
        for(int i = 0; i < 80; i++) cerr << "=";
        cerr << "\n";
    };

    print_line();
    // output symbol_table
    cerr << "SYMBOL:\n";
    for(auto &p: this->symbol_table) {
        cerr << p.first << ": " << p.second << "\n";
    }
    // output rules with id replaced by symbols using id2sym
    cerr << "RULES:\n";
    for(auto &rule: this->rules) {
        bool isFirst = true;
        for(auto &i: rule) {
            cerr << id2sym[i];
            if(isFirst) cout << "->", isFirst = false;
            // else cout << " ";
        }
        cerr << "\n";
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