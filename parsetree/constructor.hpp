#ifndef CONSTRUCTOR_HPP_INCLUDED
#define CONSTRUCTOR_HPP_INCLUDED

#include "parsetree.hpp"

using namespace parsetree;

namespace constructor{

class SyntaxTreeBranch{

    private:

    Node* _root_node;
    Node* _header_node;

    bool _compareNodes(Node n1, Node n2);

    public:

    SyntaxTreeBranch();

    SyntaxTreeBranch(Node* root_node);

    bool operator == (const SyntaxTreeBranch& stb);

    std::string toJson();

    void addChild(Node* node);

    void rmChild(int child = -2, Node* node = nullptr);

    void setHeader(int child, Node* node = nullptr);

};


class AbstractSyntacticalTree {

    private:

    Node* root_node;
    Node* header_node;

    std::map<SyntaxTreeBranch, SyntaxTreeBranch> _branch_swaps; 

    std::vector<SyntaxTreeBranch*> _find_branch(SyntaxTreeBranch template_branch);

    void _replace_branch(SyntaxTreeBranch branch);

    public:

    AbstractSyntacticalTree();

    AbstractSyntacticalTree(std::map<SyntaxTreeBranch, SyntaxTreeBranch> branch_swaps, Node* root_node);


};




};

#endif