#include <parsetree.hpp>
#include <segmenter.hpp>

using namespace std;
using namespace parsetree;
using namespace segmenter;

vector<string> readFile(std::string src) {

    string line;
    ifstream myfile(src);
    vector<string> text;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            text.push_back(line);
        }
        myfile.close();
        return text;
    } else {
        cout << "Can't open file" << endl;
        cout << "Would you like to create: " << src << " Y/n: ";
        string responce;
        cin >> responce;
        if (responce == "n") {
            vector<string> text;
            return text;
            
        } else {
            ofstream myfile(src);
            return readFile(src);
        }
    }
}

int main() {

    vector<string> raw_text = readFile("code.dots");
    vector<string> grammer_text = readFile("grammar.bnf");

    vector<Grammar> G;
    for (int i = 0; i < grammer_text.size(); i++) {
        G.push_back(Grammar(grammer_text[i]));
    }

    Segmenter segmenter(raw_text);

    segmenter.getSegments();

}