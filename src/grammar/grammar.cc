#include "../include/grammar/grammar.h"

namespace grammar
{

    const std::string kEoi = "$";
    const std::string kEps = "<EPS>";

    Grammar::Grammar(std::string filename)
    {
        std::ifstream file(filename);

        insert_terminal("<EPS>"); // register <EPS> as a special terminal symbol
        insert_terminal("$");     // register $ as end of input symbol

        std::string line;
        while (getline(file, line))
        {
            std::vector<int> rule;

            std::stringstream iss(line);
            std::string head, arrow;
            iss >> head >> arrow;

            nonterminals.insert(get_symbol_id(head));
            rule.push_back(get_symbol_id(head));

            std::string symbol;
            while (iss >> symbol)
            {
                if (symbol != "|")
                    rule.push_back(get_symbol_id(symbol));
                else
                {
                    this->rules.push_back(rule); // break the production into two rules
                    rule = {get_symbol_id(head)};
                }
            }
            this->rules.push_back(rule);
        }

        // turn all nonterminals into negative numbers
        // update rules directly
        for (auto &rule : this->rules)
        {
            for (auto &i : rule)
            {
                if (nonterminals.find(i) != nonterminals.end())
                {
                    i = -i;
                }
            }
        }
        // update nonterminal id
        std::set<int> new_nonterminals;
        for (auto i : nonterminals)
        {
            // std::cerr << "updating " << id2sym[i] << " " << i << "\n";
            symbol_table[id2sym[i]] = -i;
            id2sym[-i] = id2sym[i];
            if (i != -i)
                id2sym.erase(i);
            new_nonterminals.insert(-i);
            // std::cerr << "to " << id2sym[-i] << " " << -i << "\n";
        }
        this->nonterminals = new_nonterminals;
        // specify <S>
        this->S = *nonterminals.rbegin();
    }

    int Grammar::insert_nonterminal(const std::string &s)
    {
        // register new symbol, with id <= 0
        int id = -(this->symbol_table.size());
        this->symbol_table[s] = id;
        this->id2sym[id] = s;
        this->nonterminals.insert(id);
        return id;
    }

    int Grammar::insert_terminal(const std::string &s)
    {
        // register new symbol, with id > 0
        int id = this->symbol_table.size();
        this->symbol_table[s] = id;
        this->id2sym[id] = s;
        return id;
    }

    int Grammar::get_symbol_id(const std::string &s)
    {
        // std::cerr << "looking up symol:" << s;
        auto iter = this->symbol_table.find(s);
        if (iter == this->symbol_table.end())
        {
            // insert new symbol as terminal symbol by default
            return insert_terminal(s);
        }
        else
        {
            return iter->second;
        }
    }

    bool Grammar::inSymbolTable(symbol_t symbol)
    {
        return this->symbol_table.find(symbol) != this->symbol_table.end();
    }

    std::vector<std::vector<int>> Grammar::get_rules(int head)
    {
        std::vector<std::vector<int>> rules;
        for (auto &rule : this->rules)
        {
            if (rule[0] == head)
            {
                rules.push_back(rule);
            }
        }
        return rules;
    }

    std::vector<std::vector<int>> Grammar::get_rules()
    {
        return this->rules;
    }

    // DEBUG
    void Grammar::show()
    {
        // output a horizontal line using =
        auto print_line = []()
        {
            for (int i = 0; i < 80; i++)
                std::cerr << "=";
            std::cerr << "\n";
        };
        auto print_section = []()
        {
            for (int i = 0; i < 40; i++)
                std::cerr << "+";
            std::cerr << "\n";
        };

        print_line();
        // output symbol_table
        std::cerr << "SYMBOL:\n";
        for (auto &p : this->symbol_table)
        {
            // p: first -> string, second -> id
            if (nonterminals.find(p.second) != nonterminals.end() || p.second == 0)
                std::cerr << "(Non)" << p.first << ": " << p.second << "\n";
            else
                std::cerr << "(Ter)" << p.first << ": " << p.second << "\n";
        }
        print_section();

        // if(symbol_table.size() != id2sym.size()) {
        //     std::cerr << "size of symbol_table and id2sym = " << symbol_table.size() << " " << id2sym.size() << "\n";
        //     std::cerr << "symbol_table sync with id2sym failed!";
        //     exit(1);
        // }

        // output rules with id replaced by symbols using id2sym
        std::cerr << "RULES:\n";
        for (auto &rule : this->rules)
        {
            bool isFirst = true;
            for (auto &i : rule)
            {

                std::cerr << id2sym[i]
                    // << "[" << i << "]"
                    ;

                if (isFirst)
                    std::cout << "->", isFirst = false;
                else
                    std::cout << " ";
            }
            std::cerr << "\n";
        }
        print_section();

        std::cerr << "FIRST:\n";
        for (auto &p : this->FIRST)
        {
            std::cerr << id2sym[p.first] << ": "
                // << "[" << p.first << "]: "
                ;
            for (auto &i : p.second)
            {
                std::cerr << id2sym[i]
                    // << "[" << i << "]"
                    ;
            }
            std::cerr << "\n";
        }
        print_section();

        std::cerr << "FOLLOW:\n";
        for (auto &p : this->FOLLOW)
        {
            std::cerr << id2sym[p.first] << ": "
                // << "[" << p.first << "]: "
                ;
            for (auto &i : p.second)
            {
                std::cerr << id2sym[i]
                    // << "[" << i << "]"
                    ;
            }
            std::cerr << "\n";
        }
        print_section();

//        std::cerr << "Parsing Table:\n";
//        for (auto &p : this->parsing_table)
//        {
//            std::cerr << "(" << id2sym[p.first.first] << ","
//                      << id2sym[p.first.second] << ") -> "
//                      // std::cerr << "(" << p.first.first << ","
//                      // << p.first.second << ") -> "
//                      << p.second << ": ";
//
//            print_rule(rules[p.second]);
//        }

//        print_line();
    }

    // DEBUG
    void Grammar::print_rule(std::vector<int> r)
    {
        // print the r with id replaced by symbols using id2sym
        bool isFirst = true;
        for (auto &i : r)
        {
            std::cerr << id2sym[i];
            if (isFirst)
                std::cout << "->", isFirst = false;
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }

    // DEBUG
    void Grammar::print_item(const item_t &item)
    {
        for (int i = 0; i < this->rules[item.rule_id].size(); i++)
        {
            if (i == item.dot_pos)
            {
                std::cerr << ".";
            }
            std::cerr << id2sym[rules[item.rule_id][i]];
            if (i == 0)
                std::cerr << "->";
            else
                std::cerr << " ";
        }
        if (item.dot_pos == this->rules[item.rule_id].size())
            std::cerr << ".";
        std::cerr << "\n";
    };
} // namespace grammar