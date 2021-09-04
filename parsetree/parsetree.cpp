#include "parsetree.hpp"

namespace parsetree {

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

void Token::print(){
    std::map<std::string,std::string> map = this->data;
    
    for (auto it= map.begin(); it!=map.end(); ++it){
    std::cout << it->first << ":" << it->second << "\n";
    }
    std::cout << std::endl;
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


Node::Node(Node* parent_node_ptr, std::string type, std::map<std::string, Node*> children){
    this->_parent_node_ptr = parent_node_ptr;
    this->type = type;
    this->children = children;
}

void Node::print(int depth){
    for (int i = 0; i < depth; i++){
        std::cout << "\t";
    }
    std::cout << (std::string)this->type << ":" << std::endl;
    for (auto itr = children.begin(); itr != children.end(); itr++){
        Node* child = itr->second;
        child->print(depth+1);
    }

};


Rule::Rule(std::string bnf_string){

    Tokenizer lexer = Tokenizer();
    lexer.setString(bnf_string);

    std::map<std::string, std::string> regex_map;
    regex_map.insert(std::pair<std::string, std::string> ("LINK","^<\\w+>"));
    regex_map.insert(std::pair<std::string, std::string> ("RAW","^([a-zA-Z]|[\\+\\/\\=\\*\\^\\%\\-])+"));

    lexer.setTokenRegex(regex_map);

    Token t = lexer.getNextToken();

    

    while(!t.isEnd()){
        std::string bnf_token = t.data["value"];
        if (t.data["type"] == "LINK"){
            std::pair<std::string, std::string> d("LINK" , bnf_token.substr(1,bnf_token.size()-2));
            this->_data.push_back(d);
        }
        else if(t.data["type"] == "RAW"){
            std::pair<std::string, std::string> d("RAW" , bnf_token);
            this->_data.push_back(d);
        }
        else{
            std::pair<std::string, std::string> d("NULL","");
            this->_data.push_back(d);
        }
        t = lexer.getNextToken();
    }

}

void Rule::print(){
    for (int i = 0; i<this->_data.size(); i++){
        std::cout << this->_data[i].first << " : " << this->_data[i].second << std::endl;
    }
}

std::vector<std::pair<std::string, std::string>>::iterator Rule::begin(){
    return this->_data.begin();
}
std::vector<std::pair<std::string, std::string>>::iterator Rule::end(){
    return this->_data.end();
}

Grammar::Grammar(std::string bnf_string){
    Tokenizer lexer = Tokenizer();
    lexer.setString(bnf_string);

    std::map<std::string, std::string> regex_map;
    regex_map.insert(std::pair<std::string, std::string> ("NAME","^\\w+::"));
    regex_map.insert(std::pair<std::string, std::string> ("RULE","^(<\\w+>|([a-zA-Z]+((\\w+)|(\\d+))?)|(\\d+([a-zA-Z_])((\\w+)|(\\d+))?)|[\\+\\/\\=\\*\\^\\%\\-]|\\s+)+\\|"));

    lexer.setTokenRegex(regex_map);

    Token t = lexer.getNextToken();

    while(!t.isEnd()){
        std::string bnf_token = t.data["value"];
        if (t.data["type"] == "NAME"){
            std::string d(bnf_token.substr(0,bnf_token.size()-2));
            this->_name = d;
        }
        else if(t.data["type"] == "RULE"){
            std::string d(bnf_token.substr(0,bnf_token.size()-1));
            this->_rules.push_back(Rule(d));
        }
        else{
            this->_rules.push_back(Rule(""));
        }
        t = lexer.getNextToken();
    }
}

void Grammar::print(){
    std::cout << "<" <<this->_name << ">"<< std::endl;
    for ( int i = 0; i < _rules.size(); i++){
        std::cout << "Rule " << i+1 << ":" << std::endl;
        this->_rules[i].print();
        std::cout << std::endl;
    }

}

std::vector<Rule>::iterator Grammar::begin(){
    return this->_rules.begin();
}

std::vector<Rule>::iterator Grammar::end(){
    return this->_rules.end();
}


Tree::Tree(std::vector<Grammar> grammar, std::map<std::string, std::string> regex_map, std::string raw_text){
    this->_lexer().setString(raw_text);
}

}

    
