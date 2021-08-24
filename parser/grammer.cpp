#include "grammer.hpp"

namespace grammer{




Rule::Rule(std::string bnf_string){

    tokenizer::Tokenizer lexer = tokenizer::Tokenizer();
    lexer.setString(bnf_string);

    std::map<std::string, std::string> regex_map;
    regex_map.insert(std::pair<std::string, std::string> ("LINK","^\\<\\w+\\>"));
    regex_map.insert(std::pair<std::string, std::string> ("RAW","^\\w+"));

    lexer.setTokenRegex(regex_map);

    tokenizer::Token t = lexer.getNextToken();

    while(!t.isEnd()){
        if (t.data[]
        t = lexer.getNextToken();
    }

}

std::map<std::string, std::string>::iterator begin();
std::map<std::string, std::string>::iterator end();


};



    
}
