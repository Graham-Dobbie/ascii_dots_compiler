#include <tokenizer.hpp>
#include <iostream>

using namespace std;
using namespace tokenizer;

void print(Token t){
    
    map<string,string> map = t.data;
    
    for (auto it= map.begin(); it!=map.end(); ++it){
    std::cout << it->first << ":" << it->second << "\n";
    }
    std::cout << endl;
}

int main(){

    Tokenizer lexer = Tokenizer();

    std::map<std::string, std::string> regex_map;
    regex_map.insert(std::pair<std::string, std::string> ("NUMBER","^\\d+"));
    regex_map.insert(std::pair<std::string, std::string> ("SPACE","^\\s+"));
    regex_map.insert(std::pair<std::string, std::string> ("WORD","^\\w+"));

    lexer.setString((string)"1220 abc");
    lexer.setTokenRegex(regex_map);

    Token t = lexer.getNextToken();


    while(!t.isEnd()){
        print(t);
        t = lexer.getNextToken();
    }
    
    

}