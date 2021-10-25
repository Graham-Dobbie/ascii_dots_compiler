#include "constructor.hpp"

namespace constructor {

SyntaxTreeBranch::SyntaxTreeBranch(){
    this->_root_node = nullptr;
    this->_header_node = this->_root_node;
}


SyntaxTreeBranch::SyntaxTreeBranch(Node* root_node){
    this->_root_node = root_node;
    this->_header_node = root_node;
}

bool SyntaxTreeBranch::_compareNodes(Node n1, Node n2){
    bool b = true;
    if ((n1.type != "") and (n1.type != n2.type)){
        b = false;
    }
    else if(n1.data.size() != 0){
        
    }


}


bool SyntaxTreeBranch::operator== (const SyntaxTreeBranch& stb){

}


void SyntaxTreeBranch::addChild(Node* node){
    this->_header_node->children.push_back(node);
}


void SyntaxTreeBranch::rmChild(int child, Node* node){

    if ((node == nullptr) and (child >= 0)){
        this->_header_node->children.erase(this->_header_node->children.begin() + child);
    }
    else if (node != nullptr){
        for(int i = 0; i < this->_header_node->children.size(); i++){
            if (this->_header_node->children[i] == node){
                this->_header_node->children.erase(this->_header_node->children.begin() + i);
            }
        }

    }
    else{
        std::cout << "Node: " << node->type << "does not exist in header node" << std::endl;
    }
}


void SyntaxTreeBranch::setHeader(int child, Node* node){

    if ((node == nullptr) and (child >= 0)){
        this->_header_node = this->_header_node->children[child];
    }
    else if (node != nullptr){
        for(int i = 0; i < this->_header_node->children.size(); i++){
            if (this->_header_node->children[i] == node){
                this->_header_node = this->_header_node->children[i];
            }
        }

    }
    else{
        std::cout << "Node: " << node->type << "does not exist in header node" << std::endl;
    }
}

  
}
