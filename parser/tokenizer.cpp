#include "tokenizer.hpp"


namespace tokenizer {

Token::Token(std::string type, std::string value){
    this->data.insert(std::pair<std::string, std::string>("type", type));
    this->data.insert(std::pair<std::string, std::string>("value", value));
};

bool Token::isEnd(){
    if (this->data["type"] == "END"){
        return true;
    }
    else{
        return false;
    }
}


Token Tokenizer::getNextToken(){
    this->_string = this->_string.substr(this->_cursor, -1);
    this->_cursor = 0;
    
    if (!this->isEnd()){

        std::smatch matches;
        std::string type;
        

        bool done = false;

        for (auto it= regex_map.begin(); it!=regex_map.end(); ++it){

            std::string type = it->first;
            std::regex_search(_string, matches, it->second);

            if (!matches.empty()){
                
                std::string value = matches.str(0);

                this->_cursor += value.size();
                

                return Token(type, value);
                }

        }
        this->_cursor += _string.size();
        return Token("Null", "");
       

        
    }
    else{
        return Token("END", "");
    }
}

void Tokenizer::setString(std::string m){
    this->_string = m;
}

bool Tokenizer::isEnd(){

    if (this->_cursor == this->_string.size()){
        return true;
    }
    else{
        return false;
    }
}

void Tokenizer::setTokenRegex(std::map<std::string, std::string> map){
    for (auto it= map.begin(); it!=map.end(); ++it){
        std::string type = it->first;
        std::regex regex = std::regex(it->second);
        this->regex_map.insert(std::pair<std::string, std::regex> (type,regex));
    }
}


}