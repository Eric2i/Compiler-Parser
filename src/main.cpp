#include <iostream>
#include <vector>
#include "grammar.h"

using namespace std;

int main() {
    Grammar g("test/input/first.txt");
    vector<vector<int>> rules;

    g.eliminate_left_recursion();
    g.left_factoring();
    g.build_first();
    g.build_follow();
    g.show();

    return 0;
}

