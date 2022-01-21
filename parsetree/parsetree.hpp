#ifndef PARSETREE_HPP_INCLUDED
#define PARSETREE_HPP_INCLUDED

#include "compiler.hpp"

namespace parsetree {

// Token class stores the type and value the tokenizer scans from the source code
class Token {

  public:
    Token();

    Token(std::string type, std::string text_data);

    void print();

    std::string type;

    std::string text_data;

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

// A singular Charactor in BNF grammar
struct Character {
  public:
    Character();
    Character(std::string type, std::string value);

    bool operator==(const Character &c);
    bool operator==(const Token &t); // Used to compare tokens fore parsing

    std::string type;
    std::string value;
};

// A singular rule for the grammar of an expression made of Charactors
class Rule {

  public:
    std::vector<Character> _data;

    Rule();
    Rule(std::string bnf_string = "");
    Rule(std::vector<Character> charactors);

    bool operator==(const Rule &n);

    void print();

    std::vector<Character>::iterator begin();
    std::vector<Character>::iterator end();
};

// Grammar for a single expression made of rules
class Grammar {

  public:
    std::string _name;
    std::vector<Rule> _rules;

    Grammar();
    Grammar(std::string bnf_string);
    Grammar(std::string name, std::vector<Rule> rules);

    bool operator<(const Grammar &g);

    void print();

    std::vector<Rule>::iterator begin();
    std::vector<Rule>::iterator end();
};

// Is a parse tree node that stores relevant information
class ParseNode {
  public:
    ParseNode();
    ParseNode(ParseNode *parent_node_ptr, std::string type, std::vector<ParseNode *> children);

    std::string value;
    std::map<std::string, std::string> data;
    std::vector<ParseNode *> children;

    std::vector<ParseNode *> getChildren();
    std::string getType();

    bool isNull(); // determineds if the node is null

    void print(int depth); // recursive print

    ParseNode *_parent_node_ptr;
};

// Parse Tree class condains a root node and methods to parse different tokens
class ConcreteSyntacticalTree {
  private:
    ParseNode *_root_node;
    ParseNode *_header_node;

    std::vector<Grammar> _G;
    Grammar _root_grammar;

    std::map<Grammar, Grammar> _a_to_a_prime;

    std::map<std::string, std::string> _regex_map;
    Tokenizer _lexer;

    int _current_token;
    std::vector<Token> _tokens;

    std::pair<Grammar, Token> _error;

    ParseNode *_parse(Grammar G, Character C, int token_number); // recurisive desent parser

    void _trimTree(ParseNode *node); // trim the tree for cst to ast translation

    ParseNode *_makeDataNode(Token t); // constructs data nodes for trees

    void _makeGrammar(); // removes left recursion in the BNF grammer

    void _helpPrint(ParseNode start_node, int depth);

    void _mergePrimes(ParseNode *node); // helps trim tree

    void _ridEpsilons(ParseNode *node); // helps trim tree

  public:
    ConcreteSyntacticalTree();
    ConcreteSyntacticalTree(std::vector<Grammar> grammar, std::map<std::string, std::string> regex_map, std::string raw_text);

    Grammar getGrammar(Character c);

    void makeTree(); // uses lexer and grammar rules to make a concrete syntactical tree of nodes

    ParseNode *getTreeHeader(); // returns the root node of the tree

    void readF(std::string src); // work in progress

    void writeF(std::string dst = "parser_cst.json"); // work in progress

    void print(); //

    void printGrammar();
};

bool operator<(const Grammar &g1, const Grammar &g2);
}; // namespace parsetree
#endif