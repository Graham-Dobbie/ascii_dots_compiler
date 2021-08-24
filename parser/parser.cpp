#include "parser.hpp"

namespace parser{

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

}



}