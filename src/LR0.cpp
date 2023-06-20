#include <iostream>
#include <vector>
#include <fstream>
#include "grammar.h"

using namespace std;

int main() {
    grammar::path RULES = "test/input/LR0_rules.txt";
    grammar::path TOKENS = "test/input/LR0_rules_lang.txt";

    // read grammar from .txt file
    grammar::LR0Parser g(RULES);

    g.augmentate();
    g.build_dfa();
    g.show_dfa();

    // get pseudotokens from file
    ifstream fin(TOKENS);
    vector<string> tokens;
    string token;
    while(fin >> token) {
        tokens.push_back(token);
    }
    if(g.parse(tokens)) cout << "No Errors!\n";
    else cout << "Errors Exist!\n";

    return 0;
}