#include <iostream>
#include <vector>
#include "grammar.h"

using namespace std;

int main() {
    struct Grammar g;
    vector<vector<int>> rules;

    g.read_grammar("test/input/left_recursion.txt");
    g.eliminate_left_recursion();
    // g.show();

    return 0;
}

