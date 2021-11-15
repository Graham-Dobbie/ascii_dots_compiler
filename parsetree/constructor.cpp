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
    if (n2.value != ""){
        if (n1.value != n2.value){
            b = false;
        }
    }
    else if ((n1.data.count("type") != 0) and (n1.data.count("type") != 0)) {
        std::string n1_type = n1.data["type"];
        std::string n2_type = n2.data["type"];
        if (n1_type != n2_type){
            b = false;
        }
    } 
    return b;
}


bool SyntaxTreeBranch::operator == (const SyntaxTreeBranch& stb){
    bool b = true;
    SyntaxTreeBranch stb_c = stb;

    if (this->_header_node == stb_c._header_node){

        bool finished = false;
        bool good = true;
        int i = 0;
        while (finished){
            this->setHeader(i);
            stb_c.setHeader(i);

            if(*this == stb_c){
                this->headerBack();
                stb_c.headerBack();
                if ((i == this->_header_node->children.size()) and (i == stb_c._header_node->children.size())){
                    finished = true;
                }
                else if ((i == this->_header_node->children.size()-1) xor (i != stb_c._header_node->children.size()-1)){
                    finished = true;
                    good = false;
                }
            }
            
            else{
                finished = true;
                good = false;
            }
        }
        return good;
    }
    else{
        return false;
    }
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
        std::cout << "Node: " << node->value << "does not exist in header node" << std::endl;
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
        std::cout << "Node: " << node->value << "does not exist in header node" << std::endl;
    }
}

void SyntaxTreeBranch::headerBack(){ 
    this->_header_node = this->_header_node->_parent_node_ptr;
}

AbstractSyntacticalTree::AbstractSyntacticalTree(){
    this->root_node = nullptr;
    this->header_node = this->root_node;
}

// std::vector<SyntaxTreeBranch*> AbstractSyntacticalTree::_helper_find_branch(SyntaxTreeBranch template_branch, std::vector<SyntaxTreeBranch*> results){

// }

// std::vector<SyntaxTreeBranch*> AbstractSyntacticalTree::find_branch(SyntaxTreeBranch template_branch){
//     if(this.header)
// }

  
}
