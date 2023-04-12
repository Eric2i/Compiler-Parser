#include <iostream>
#include <vector>
#include <fstream>
#include "grammar.h"

using namespace std;

int main() {
    Grammar g("test/input/grammar.txt");
    vector<vector<int>> rules;

    g.eliminate_left_recursion();
    g.left_factoring();
    g.build_first();
    g.build_follow();
    g.build_parsing_table();
    g.show();

    ifstream fin("test/input/tokens.txt");
    vector<string> tokens;
    string token;
    while(fin >> token) {
        tokens.push_back(token);
    }
    if(g.parse(tokens)) cout << "Match!";
    else cout << "Mismatch!";

    return 0;
}

