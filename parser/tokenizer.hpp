#ifndef TOKENIZER_HPP_INCLUDED
#define TOKENIZER_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include <iterator>

namespace tokenizer {

// Token class stores the type and value the tokenizer scans from the source code
class Token {

    public:

    Token();

    Token(std::string type, std::string value);

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
}

#endif 