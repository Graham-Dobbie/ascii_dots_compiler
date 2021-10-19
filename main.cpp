#include <parsetree.hpp>

using namespace std;
using namespace parsetree;

vector<Grammar> readGrammar(std::string src = "grammar.bnf"){
    
    string line;
    ifstream myfile (src);
    vector<Grammar> G;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            Grammar g = Grammar(line);
            G.push_back(g);
        }
        myfile.close();
        return G;
  }
  else{
      cout << "Can't open file" << endl;
      cout << "Would you like to create: " << src << " Y/n:";
      string responce;
      cin >> responce;
      if (responce == "Y"){
            ofstream myfile (src);
            return readGrammar();
      }
      else{
          vector<Grammar> v;
          return v ;
      }
  }
}


int main(){

    Tokenizer lexer = Tokenizer();

    std::map<std::string, std::string> regex_map;
    regex_map.insert(std::pair<std::string, std::string> ("NUMBER","^\\d+"));
    regex_map.insert(std::pair<std::string, std::string> ("SPACE","^\\s+"));
    regex_map.insert(std::pair<std::string, std::string> ("IDENT","^([a-zA-Z]+((\\w+)|(\\d+))?)|(\\d+([a-zA-Z_])((\\w+)|(\\d+))?)"));
    regex_map.insert(std::pair<std::string, std::string> ("OPERATOR","^[\\+\\/\\=\\*\\^\\%\\-]"));
    regex_map.insert(std::pair<std::string, std::string> ("PRESEDENCE_SIGNIFIER","^[\\(\\)\\[\\]]"));
    regex_map.insert(std::pair<std::string, std::string> ("START","^<start>"));
    

    string raw_text;
    cin >> raw_text;
    cout << endl;
    
    vector<Grammar> G = readGrammar();

    ConcreteSyntacticalTree cst = ConcreteSyntacticalTree(G, regex_map,raw_text);

    cst.makeTree();

    cst.print();
}