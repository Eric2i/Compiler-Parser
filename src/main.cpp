#include <iostream>
#include <vector>
#include "grammar.h"

using namespace std;

int main() {
    struct Grammar g;
    vector<vector<int>> rules;

    // g.read_grammar("test/input/first_nonrecursion.txt");
    g.read_grammar("test/input/first.txt");
    g.eliminate_left_recursion();
    g.left_factoring();
    g.build_first();
    g.show();

    return 0;
}

