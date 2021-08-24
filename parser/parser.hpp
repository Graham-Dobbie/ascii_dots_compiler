#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <tokenizer.hpp>

namespace parser {


class Node {
    public:
    Node (Node* parent_node_ptr, std::string type, std::map<std::string, Node*> children);
    
    std::string type;
    std::map<std::string, Node*> children;

    std::map<std::string, Node*>  getChildren();
    std::string getType();

    void print(int depth);

    private:
    Node* _parent_node_ptr;
    

};




class Tree{

    Node _root_node;


};
    
};




#endif 