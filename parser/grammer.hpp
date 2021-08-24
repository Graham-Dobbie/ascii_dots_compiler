#ifndef GRAMMER_HPP_INCLUDED
#define GRAMMER_HPP_INCLUDED

#include <tokenizer.hpp>

namespace grammer {

class Rule {

    private:

    std::map<std::string, std::string>_data;

    public:

    Rule(std::string bnf_string);

    std::map<std::string, std::string>::iterator begin();
    std::map<std::string, std::string>::iterator end();


};

// struct Grammer {

//     GrammerRule()
    

// }






}

#endif