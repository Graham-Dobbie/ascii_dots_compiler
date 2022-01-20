#include "parsetree.hpp"

namespace parsetree {

Token::Token() {
    this->type = "END";
    this->text_data = "";
}

Token::Token(std::string type, std::string text_data) {
    this->type = type;
    this->text_data = text_data;
}

bool Token::isEnd() {
    if (this->type == "END") {
        return true;
    } else {
        return false;
    }
}

void Token::print() {

    std::cout << this->type << ":" << this->text_data << "\n";
    std::cout << std::endl;
}

Tokenizer::Tokenizer(std::map<std::string, std::regex> regex_map, std::string m) {
    this->_regex_map = regex_map;
    this->_string = m;
}

Tokenizer::Tokenizer() {}

Token Tokenizer::getNextToken() {
    this->_string = this->_string.substr(this->_cursor, -1);
    this->_cursor = 0;

    if (!this->isEnd()) {

        std::smatch matches;
        std::string type;

        bool done = false;

        for (auto it = _regex_map.begin(); it != _regex_map.end(); ++it) {

            std::string type = it->first;
            std::regex_search(_string, matches, it->second);

            if (!matches.empty()) {

                std::string value = matches.str(0);

                this->_cursor += value.size();

                return Token(type, value);
            }
        }
        this->_cursor += _string.size();
        return Token("Null", "");
    } else {
        return Token("END", "");
    }
}

void Tokenizer::setString(std::string m) { this->_string = m; }

bool Tokenizer::isEnd() {

    if (this->_cursor == this->_string.size()) {
        return true;
    } else {
        return false;
    }
}

void Tokenizer::setTokenRegex(std::map<std::string, std::string> map) {
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::string type = it->first;
        std::regex regex = std::regex(it->second);
        this->_regex_map.insert(std::pair<std::string, std::regex>(type, regex));
    }
}

ParseNode::ParseNode(){
    this->_parent_node_ptr = nullptr;
    this->value = "NULL";
}

ParseNode::ParseNode(ParseNode* parent_node_ptr, std::string value, std::vector<ParseNode*> children){
    this->_parent_node_ptr = parent_node_ptr;
    this->value = value;
    this->children = children;
}

void ParseNode::print(int depth){
    for (int i = 0; i < depth; i++){
        std::cout << "--";
    }
    std::cout << (std::string)this->value << ":" << std::endl;
    for (int i =0; i < this->children.size(); i++){
        ParseNode* child = this->children[i];
        child->print(depth+1);
    }

};

bool ParseNode::isNull(){
    if(this->value == "NULL"){
        return true;
    }
    else{
        return false;
    }
}

Character::Character() {}
Character::Character(std::string type, std::string value) {
    this->type = type;
    this->value = value;
}

Rule::Rule() {}

Rule::Rule(std::string bnf_string) {

    Tokenizer lexer = Tokenizer();
    lexer.setString(bnf_string);

    std::map<std::string, std::string> regex_map;
    regex_map.insert(std::pair<std::string, std::string>("LINK", "^<\\w+>"));
    regex_map.insert(std::pair<std::string, std::string>("RAW", "^([a-zA-Z]|[\\+\\/\\=\\*\\^\\%\\-\\(\\)])+"));

    lexer.setTokenRegex(regex_map);

    Token t = lexer.getNextToken();

    while (!t.isEnd()) {
        std::string bnf_token = t.text_data;
        if (t.type == "LINK") {
            Character c("LINK", bnf_token.substr(1, bnf_token.size() - 2));
            this->_data.push_back(c);
        } else if (t.type == "RAW") {
            Character c("RAW", bnf_token);
            this->_data.push_back(c);
        } else {
            Character c("NULL", "");
            this->_data.push_back(c);
        }
        t = lexer.getNextToken();
    }
}

bool Rule::operator==(const Rule &n) {
    auto itr = this->begin();
    if (itr->type == "LINK") {
        return false;
    }
    Rule n0 = n;
    auto itr0 = n0.begin();
    while (itr != this->end()) {
        if (itr.base() != itr0.base()) {
            return false;
        }
        itr++;
        itr0++;
    }

    return true;
};

Rule::Rule(std::vector<Character> characters) { this->_data = characters; }

void Rule::print() {
    for (int i = 0; i < this->_data.size(); i++) {
        std::cout << this->_data[i].type << " : " << this->_data[i].value << std::endl;
    }
}

std::vector<Character>::iterator Rule::begin() { return this->_data.begin(); }
std::vector<Character>::iterator Rule::end() { return this->_data.end(); }

Grammar::Grammar() {
    this->_name = "NULL";
    this->_rules.push_back(Rule(""));
}

Grammar::Grammar(std::string bnf_string) {
    Tokenizer lexer = Tokenizer();
    lexer.setString(bnf_string);

    std::map<std::string, std::string> regex_map;
    regex_map.insert(std::pair<std::string, std::string>("NAME", "^\\w+::"));
    regex_map.insert(std::pair<std::string, std::string>("RULE", "^(<\\w+>|([a-zA-Z]+((\\w+)|(\\d+))?)|(\\d+([a-zA-Z_])((\\w+)|(\\d+))?)|[\\+\\/\\=\\*\\^\\%\\-\\(\\)]|\\s+)+\\|"));

    lexer.setTokenRegex(regex_map);

    Token t = lexer.getNextToken();

    while (!t.isEnd()) {
        std::string bnf_token = t.text_data;
        if (t.type == "NAME") {
            std::string d(bnf_token.substr(0, bnf_token.size() - 2));
            this->_name = d;
        } else if (t.type == "RULE") {
            std::string d(bnf_token.substr(0, bnf_token.size() - 1));
            this->_rules.push_back(Rule(d));
        } else {
            this->_rules.push_back(Rule(""));
        }
        t = lexer.getNextToken();
    }
}

Grammar::Grammar(std::string name, std::vector<Rule> rules) {
    this->_name = name;
    this->_rules = rules;
}

bool Grammar::operator<(const Grammar &g) { return this->_name.size() < g._name.size(); }

void Grammar::print() {
    std::cout << "<" << this->_name << ">" << std::endl;
    for (int i = 0; i < _rules.size(); i++) {
        std::cout << "Rule " << i + 1 << ":" << std::endl;
        this->_rules[i].print();
        std::cout << std::endl;
    }
}

std::vector<Rule>::iterator Grammar::begin() { return this->_rules.begin(); }

std::vector<Rule>::iterator Grammar::end() { return this->_rules.end(); }

ConcreteSyntacticalTree::ConcreteSyntacticalTree(){
    this->_lexer.setString("");
    std::map<std::string, std::string> regex;
    this->_lexer.setTokenRegex(regex);
    ParseNode *n = new ParseNode();
    this->_root_node = n;

    this->_a_to_a_prime;

    this->_root_grammar = Grammar();
    this->_header_node = this->_root_node;

    this->_tokens.push_back(Token("END", ""));

}

ConcreteSyntacticalTree::ConcreteSyntacticalTree(std::vector<Grammar> grammar, std::map<std::string, std::string> regex_map, std::string raw_text){
    raw_text = "<start>" + raw_text;
    this->_lexer.setString(raw_text);

    this->_lexer.setTokenRegex(regex_map);

    ParseNode *n = new ParseNode();
    this->_root_node = n;

    this->_G = grammar;

    this->_root_grammar = this->_G[0];
    this->_header_node = this->_root_node;

    this->_current_token = 0;

    this->_tokens.push_back(this->_lexer.getNextToken());

    this->_a_to_a_prime;

    this->_makeGrammar();
}

ParseNode* ConcreteSyntacticalTree::_makeDataNode(Token t){
    ParseNode *child_node = new ParseNode();
        child_node->_parent_node_ptr = this->_header_node;
        child_node->value = t.type;

        ParseNode *data_node = new ParseNode();
        data_node->_parent_node_ptr = child_node;

        child_node->children.push_back(data_node);
        return child_node;
}

ParseNode* ConcreteSyntacticalTree::_parse(Grammar G, Character C, int t ){

    int initial_token = t;
    if (C.type == "RAW"){
        if (C == this->_tokens[this->_current_token]){
            ParseNode* n = _makeDataNode(this->_tokens[this->_current_token]);
            this->_current_token++;
            if (this->_current_token >= this->_tokens.size()-1){
                this->_tokens.push_back( this->_lexer.getNextToken());
            }
            return n;

        }
        else{
            return new ParseNode();
        }
    }else if (C.type == "LINK"){
        std::vector<ParseNode*> children;

        ParseNode* return_node = new ParseNode();

        return_node->value = G._name;

        ParseNode* old_header = this->_header_node;
        return_node->_parent_node_ptr = old_header;

        this->_header_node = return_node;

        for(int i = 0; i < G._rules.size(); i++){
            Rule r = G._rules[i];
            children.clear();

            int j = 0;
            bool finished = false;

            while ((j <=  r._data.size()-1) and (!finished)){

                Character c_temp = r._data[j];
                Grammar g_temp = this->getGrammar(c_temp);

                ParseNode* n_temp = this->_parse(g_temp, c_temp, this->_current_token);
                if(n_temp->isNull()){
                    children.clear();
                    finished = true;
                    this->_current_token = initial_token;
                }
                else{
                    children.push_back(n_temp);
                }
                j++;
            }

            if (children.size() != 0){
                this->_header_node = old_header;
                return_node->children = children;
                this->_error = std::pair<Grammar,Token> (Grammar(), Token());
                return return_node;
            }
        }

        return new ParseNode();
        if (this->_error.first._name == "NULL"){
            this->_error = std::pair<Grammar,Token> (G, this->_tokens[this->_current_token]);
        }

    }
    else if (C.type == "EPSILON"){
        std::vector<ParseNode*> c;
        return new ParseNode(this->_header_node, "END", c);
    }
    else{
        return new ParseNode();
    }

}

Grammar ConcreteSyntacticalTree::getGrammar(Character c){
    for(int i = 0; i < this->_G.size(); i++){
        if (this->_G[i]._name == c.value){
            return this->_G[i];
        }

    }
    return Grammar("");
}

void ConcreteSyntacticalTree::makeTree(){
    Character root_char = Character("LINK", this->_root_grammar._name);

    this->_error = std::pair<Grammar,Token> (Grammar(), Token());

    ParseNode* n = _parse(this->_root_grammar, root_char, 0);

    if (n->value == "NULL"){
        std::cout << "syntax error:" << std::endl;
        _error.first.print();
        _error.second.print();
    }

    _trimTree(n);

    this->_root_node = n;
}

void ConcreteSyntacticalTree::_makeGrammar(){
    std::vector<Grammar> temp_grammar; //make grammer to be returned
    int id = 0;

    for (int i = 0; i < _G.size(); i++){
        Grammar g = _G[i];

        std::vector<Rule> A_alpha_rules; // reccursive rules
        std::vector<Rule> beta_rules;  //non recursive rules

        for (int i = 0; i < g._rules.size(); i++){ //detects reccursive rule and spilts them

            Rule r = g._rules[i];

            if((r._data[0].type == "LINK") and (r._data[0].value == g._name)){
                A_alpha_rules.push_back(r);
            }
            else{
                beta_rules.push_back(r);
            }
        }

        std::vector<Rule> a_rules;
        std::vector<Rule> a_prime_rules;

        for(int i = 0; i < A_alpha_rules.size(); i++){ //turns recursive rules into alphas and removes recursion

            Rule alpha = A_alpha_rules[i];
            alpha._data.erase(alpha._data.begin());
            a_prime_rules.push_back(alpha);

        }

        Character e("EPSILON", "NULL"); //adds an end to the grammar
        std::vector<Character> ep;
        ep.push_back(e);
        Rule epsilon(ep);
        a_prime_rules.push_back(epsilon);

        for(int j = 0; j < beta_rules.size(); j++){ // create the initial entry to the grammar with the beta rules
                Rule a = beta_rules[j];
                a._data.push_back(Character("LINK", std::to_string(id)));
                a_rules.push_back(a);

        }

        Grammar A(g._name, a_rules);
        Grammar A_prime(std::to_string(id), a_prime_rules);

        temp_grammar.push_back(A);
        temp_grammar.push_back(A_prime);

        std::pair< Grammar, Grammar > pair(A, A_prime);

        this->_a_to_a_prime.insert(pair);

        id++;
    }

    this->_G = temp_grammar;
}

void ConcreteSyntacticalTree::_mergePrimes(ParseNode* node){
        std::vector<ParseNode*> c = node->children;

    Grammar h_g;
    Grammar h_p_g;

    for (auto itr = this->_a_to_a_prime.begin(); itr != this->_a_to_a_prime.end(); itr++ ){
        if(node->value == itr->first._name){
          h_g = itr->first;
          h_p_g = itr->second;
        }

    }

    for(int i = 0; i < c.size(); i++){

        if (c.size() > 0){
            ParseNode* child = c[i];

            if(child->value == h_p_g._name){
                node->children.erase(node->children.begin() + i);

                for(int j = 0; j < child->children.size(); j++){
                    node->children.push_back(child->children[j]);
                }
            }

            _mergePrimes(child);
        }

    }
}

void ConcreteSyntacticalTree::_ridEpsilons(ParseNode* node){

    ParseNode* parent = node->_parent_node_ptr;

    if ( node->value == "END" ){
        for(int i = 0; i < parent->children.size(); i++){

            if(node->value == parent->children[i]->value){

                parent->children.erase(parent->children.begin() + i);
            }
        }
    }

    else if (node->children.size() > 0 ){
        ParseNode* child = node->children[0];

        if ((child->value == "END") or (node->value == "END")){

            for(int i = 0; i < parent->children.size(); i++){

                if(node->value == parent->children[i]->value){

                    parent->children.erase(parent->children.begin() + i);
                }
            }
        }

        else{
            for(int i = 0; i < node->children.size(); i++){
                _ridEpsilons(node->children[i]);
            }
        }
    }
}

void ConcreteSyntacticalTree::_trimTree(ParseNode* node){
    this->_mergePrimes(node);
    this->_ridEpsilons(node);
}

ParseNode* ConcreteSyntacticalTree::getTreeHeader(){
    return this->_root_node;
}

void ConcreteSyntacticalTree::print(){
    this->_root_node->print(0);
}

void ConcreteSyntacticalTree::printGrammar(){
    for(int i = 0; i < this->_G.size(); i++){
        _G[i].print();
    }
}

bool operator < (const Grammar& g1, const Grammar& g2){
    return g1._name.size() < g2._name.size();
}

} // namespace parsetree
