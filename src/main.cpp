#include <iostream>
#include <vector>
#include "grammar.h"

using namespace std;

int main() {
    struct Grammar g;
    vector<vector<int>> rules = g.read_grammar("test/input/grammar.txt");

    for (size_t i = 0; i < rules.size(); i++) {
        for (size_t j = 0; j < rules[i].size(); j++) {
            if(j == 0) cout << rules[i][j] << " -> ";
            else cout << rules[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}

