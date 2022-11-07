//
// Created by 10578 on 10/9/2019.
//

#include "ast.hpp"
#include <iostream>
#include <fstream>
namespace SPL {
    
    void print_ast(Node *node, int level) {
        if (node->type.compare("empty") == 0) {
            return;
        }

        std::ofstream outfile;
        outfile.open("out.txt",std::ios::app);
        
        if(node->value.compare("INT")==0||node->value.compare("FLOAT")==0||node->value.compare("CHAR")==0||node->value.compare("TYPE")==0||node->value.compare("ID")==0)
            outfile << std::string(2 * level, ' ')<< node->type <<": "<<node->value<< std::endl;
        else 
            outfile << std::string(2 * level, ' ')<< node->type <<" ("<<std::to_string(node->line_no)<<")"<< std::endl;

        for (auto &child: node->children) {
            print_ast(child, level + 1);
        }
        
    }
    
}
