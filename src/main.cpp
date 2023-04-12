#include <iostream>
#include <vector>
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

    return 0;
}

