#include <iostream>
#include <vector>
#include <fstream>
#include "grammar.h"

using namespace std;

int main() {
    grammar::path RULES = "test/input/ll0_rules.txt";
    grammar::path SRC = "test/input/ll0_rules_lang.txt";

    // read grammar from .txt file
    grammar::LL1Parser g(RULES);

    g.eliminate_left_recursion();
    g.left_factoring();
    g.build_first();
    g.build_follow();
    g.build_parsing_table();
    g.show();

    // get pseudotokens from file
    ifstream fin(SRC);
    vector<string> tokens;
    string token;
    while(fin >> token) {
        tokens.push_back(token);
    }
    if(g.parse(tokens)) cout << "No Errors!\n";
    else cout << "Errors Exist!\n";

    return 0;
}