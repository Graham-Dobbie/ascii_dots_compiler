#include <parsetree.hpp>




using namespace std;
using namespace parsetree;


int main(){

    Tokenizer lexer = Tokenizer();

    std::map<std::string, std::string> regex_map;
    regex_map.insert(std::pair<std::string, std::string> ("NUMBER","^\\d+"));
    regex_map.insert(std::pair<std::string, std::string> ("SPACE","^\\s+"));
    regex_map.insert(std::pair<std::string, std::string> ("IDENTIFIER","^([a-zA-Z]+((\\w+)|(\\d+))?)|(\\d+([a-zA-Z_])((\\w+)|(\\d+))?)"));
    regex_map.insert(std::pair<std::string, std::string> ("OPERATOR","^[\\+\\/\\=\\*\\^\\%\\-]"));
    regex_map.insert(std::pair<std::string, std::string> ("PRESEDENCE_SIGNIFIER","^[\\(\\)\\[\\]]"));

    // string responce;
    // cout << ">> ";
    // cin >> responce;
    // cout << endl;
    // lexer.setString((string)"(x+4)/7");
    // lexer.setTokenRegex(regex_map);

    // Token t = lexer.getNextToken();


    // while(!t.isEnd()){
    //     t.print();
    //     t = lexer.getNextToken();
    // }
    
    Grammer g = Grammer("E::<E>+<T>|<E>-<T>|<T>|");
    g.print();

}