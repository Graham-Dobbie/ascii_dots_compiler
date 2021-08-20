#ifndef TOKENIZER_HPP_INCLUDED
#define TOKENIZER_HPP_INCLUDED

#include <iostream>
#include <vector>

namespace tokenizer {


struct Token {

    public:

    Token(std::string type, std::string value);

    std::string type;
    std::string value;

};

class Tokenizer {

    public:

    int _cursor = 0;

    std::string _string;

    Token getNextToken();

};
}

#endif 