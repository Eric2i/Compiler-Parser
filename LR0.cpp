#include <iostream>
#include <vector>
#include <fstream>
#include "src/include/grammar/lr0.h"

using namespace std;

int main() {
    const grammar::path grammar_rules_file_path = "../test/grammar/input/LR0_rules.txt";
    const grammar::path sample_tokens_file_path = "../test/grammar/input/LR0_rules_lang.txt";

    // read grammar from .txt file
    grammar::LR0Parser g(grammar_rules_file_path);

    g.augmentate();
    g.build_dfa();
    g.show_dfa();

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