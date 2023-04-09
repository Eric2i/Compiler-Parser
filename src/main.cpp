#include <iostream>
#include <vector>
#include "grammar.h"

using namespace std;

int main() {
    struct Grammar g;
    vector<vector<int>> rules;

    g.read_grammar("test/input/left_factoring.txt");
    g.eliminate_left_recursion();
    g.left_factoring();
    g.show();

    return 0;
}

