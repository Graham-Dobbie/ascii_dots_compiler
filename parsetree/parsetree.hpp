#ifndef PARSETREE_HPP_INCLUDED
#define PARSETREE_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include <iterator>
#include <fstream>
#include <unordered_map>


namespace parsetree{



// Token class stores the type and value the tokenizer scans from the source code
class Token {

    public:

    Token();

    Token(std::string type, std::string value);

    void print();

    std::string type;
    std::string value;

    bool isEnd();
};


// The Tokenizer class scans source code and separates it into tokens
class Tokenizer {
    
    private:

    int _cursor = 0;
    std::string _string;
    std::map<std::string, std::regex> _regex_map;

    public:

    Tokenizer();
    Tokenizer(std::map<std::string, std::regex> regex_map, std::string m);

    void setString(std::string m); // Gives sources code to tokenizer

    void setTokenRegex(std::map<std::string, std::string> map); // Gives a map of tokens and regex to find tokens

    Token getNextToken(); // Returns the next token of the source code 

    bool isEnd(); // Returns true if the source code is finished

};





struct Character
{   
    public:
    Character();
    Character(std::string type, std::string value);

    bool operator == (const Character& c);
    bool operator == (const Token& t);

    std::string type;
    std::string value;
};

// A singular rule for the grammar of an expression
class Rule {

    public:

    std::vector<Character>_data;

    Rule();
    Rule(std::string bnf_string = "");
    Rule(std::vector<Character> charactors);

    bool operator == (const Rule& n);

    void print();

    std::vector<Character>::iterator begin();
    std::vector<Character>::iterator end();


};


// Grammar for a single expression
class Grammar {
    

    public:
    std::string _name;
    std::vector<Rule> _rules;

    Grammar();
    Grammar(std::string bnf_string);
    Grammar(std::string name, std::vector<Rule> rules);

    bool operator<(const Grammar& g);

    void print();

    std::vector<Rule>::iterator begin();
    std::vector<Rule>::iterator end();
    

};

// Is a parse tree node that stores relevant information
class Node {
    public:
    Node();
    Node (Node* parent_node_ptr, std::string type, std::vector<Node*> children);
    
    std::string type;
    std::vector<Node*> children;

    std::vector<Node*>  getChildren();
    std::string getType();

    bool isNull();

    void print(int depth);

    Node* _parent_node_ptr;
};


// Parse Tree class condains a root node and methods to parse different tokens
class ConcreteSyntacticalTree{
    private:

    Node* _root_node;
    Node* _header_node;

    std::vector<Grammar> _G;
    Grammar _root_grammar;

    

    std::map<Grammar,Grammar> _a_to_a_prime;

    std::map<std::string, std::string> _regex_map;
    Tokenizer _lexer;

    int _current_token;
    std::vector<Token> _tokens;



    Node* _parse(Grammar G, Character C, int token_number);

    Node _parseHelper(Token token_1, Token token_2, Token token_help, Grammar G, Rule R);

    void _trimTree(Node* node);

    Node* _makeDataNode(Token t);

    void _makeGrammar();

    void _helpPrint(Node start_node, int depth);

    void _merge_primes(Node* node);

    void _rid_epsilons(Node* node);
    

    public:

    ConcreteSyntacticalTree();
    ConcreteSyntacticalTree(std::vector<Grammar> grammar, std::map<std::string, std::string> regex_map, std::string raw_text);

    Grammar getGrammar (Character c);

    void makeTree();



    Node* getTreeHeader();

    void readF(std::string src);

    void writeF(std::string dst = "parser_cst.json");

    void print();

    void print_grammar();
    

};


bool operator < (const Grammar& g1, const Grammar& g2);

// class AbstractSyntacticalTree {


// };



};



#endif 