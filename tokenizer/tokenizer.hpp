#ifndef TOKENIZER_HPP_INCLUDED
#define TOKENIZER_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>

namespace tokenizer {


class Token {

    public:

    Token();

    Token(std::string type, std::string value);

    std::map<std::string, std::string> data;

    bool isEnd();
};

class Tokenizer {

    public:

    int _cursor = 0;
    std::string _string;
    std::map<std::string, std::regex> regex_map;

    void setString(std::string m);

    void setTokenRegex(std::map<std::string, std::string> map);

    Token getNextToken();

    bool isEnd();

};
}

#endif 