#include "grammar.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

vector<vector<int>> Grammar::read_grammar(string filename) {
    ifstream file(filename);

    this->symbol_table["<S>"] = 0;
    auto get_symbol_id = [&](const string &s) {
        // cerr << "looking up " << s;
        auto iter = this->symbol_table.find(s);
        if (iter == this->symbol_table.end()) {
            int id = this->symbol_table.size();
            this->symbol_table[s] = id;
            // cerr << "with returned " << id << "\n";
            return id;
        } else {
            // cerr << "with returned " << iter->second << "\n";
            return iter->second;
        }
    };

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
    for(auto &rule : this->rules) {
        for(auto &i : rule) {
            if(nonterminals.find(i) != nonterminals.end()) i = -i;
            // else cerr << "termials (int:)" << i << '\n';
        }
    }

    return this->rules;
}

