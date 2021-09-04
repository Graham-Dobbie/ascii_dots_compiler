#ifndef PARSETREE_HPP_INCLUDED
#define PARSETREE_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include <iterator>


namespace parsetree{
// Token class stores the type and value the tokenizer scans from the source code
class Token {

    public:

    Token();

    Token(std::string type, std::string value);

    void print();

    std::map<std::string, std::string> data;

    bool isEnd();
};


// The Tokenizer class scans source code and separates it into tokens
class Tokenizer {

    public:

    int _cursor = 0;
    std::string _string;
    std::map<std::string, std::regex> regex_map;

    void setString(std::string m); // Gives sources code to tokenizer

    void setTokenRegex(std::map<std::string, std::string> map); // Gives a map of tokens and regex to find tokens

    Token getNextToken(); // Returns the next token of the source code 

    bool isEnd(); // Returns true if the source code is finished

};




class Node {
    public:
    Node (Node* parent_node_ptr, std::string type, std::map<std::string, Node*> children);
    
    std::string type;
    std::map<std::string, Node*> children;

    std::map<std::string, Node*>  getChildren();
    std::string getType();

    void print(int depth);

    private:
    Node* _parent_node_ptr;
};




class Tree{
    private:

    Node _root_node;
    std::vector<Grammar> _G;
    std::map<std::string, std::string> _regex_map;
    
    Tokenizer _lexer();

    Node _disect(Token token_1, Token token_2, Rule r);

    public:

    Tree(std::vector<Grammar> grammar, std::map<std::string, std::string> regex_map, std::string raw_text);

    void parse();

    Node getTreeHeader();

    void readF(std::string src);

    void writeF(std::string dst = "parser_cst.json");

};
    




class Rule {

    private:

    std::vector<std::pair<std::string, std::string>>_data;

    public:

    Rule(std::string bnf_string);

    void print();

    std::vector<std::pair<std::string, std::string>>::iterator begin();
    std::vector<std::pair<std::string, std::string>>::iterator end();


};

class Grammar {
    std::string _name;
    std::vector<Rule> _rules;

    public:

    Grammar(std::string bnf_string);

    void print();

    std::vector<Rule>::iterator begin();
    std::vector<Rule>::iterator end();
    

};

}

#endif 