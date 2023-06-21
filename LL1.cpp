#include <iostream>
#include <vector>
#include <fstream>
#include "src/include/grammar/ll1.h"

using namespace std;

int main() {
    grammar::path grammar_rules_file_path = "../test/grammar/input/ll0_rules.txt";
    grammar::path sample_tokens_file_path = "../test/grammar/input/ll0_rules_lang.txt";

    // read grammar from .txt file
    grammar::LL1Parser g(grammar_rules_file_path);

    g.eliminate_left_recursion();
    g.left_factoring();
    g.build_first();
    g.build_follow();
    g.build_parsing_table();
    g.show();

    // get sample tokens from file
    ifstream fin(sample_tokens_file_path);
    vector<string> tokens;
    string token;
    while(fin >> token) {
        tokens.push_back(token);
    }
    if(g.parse(tokens)) cout << "No Errors!\n";
    else cout << "Errors Exist!\n";

    return 0;
}