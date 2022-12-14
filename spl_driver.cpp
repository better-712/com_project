//
// Created by 10578 on 10/9/2019.
//

#include <cctype>
#include <fstream>
#include <cassert>
#include <algorithm>

#include "spl_driver.hpp"


namespace SPL {

    SPL_Driver::~SPL_Driver() {}

    void SPL_Driver::parse(const char *filename) {
        assert(filename != nullptr);
        std::ifstream in_file(filename);
        parse_helper(in_file);
    }

    

    void SPL_Driver::parse_helper(std::istream &is) {
       
        scanner = new SPL_Scanner(&is);
        
        parser = new SPL_Parser(*scanner, *this);
        try {
            parser->parse();
        } catch (const SPL_Parser::syntax_error &e) {
           
        }
    }
    void SPL_Driver::set_root(Node *node){
        root = node;
    }
    Node* SPL_Driver::get_root(){
        return root;
    }
    void SPL_Driver::add_syntax_error(std::string lexeme, Scan_Info *info) {
        syntax_errors->push_back(new Syntax_Error(lexeme, info->line_no));
    }
    void SPL_Driver::add_syntax_error(std::string lexeme, Node *info) {
        syntax_errors->push_back(new Syntax_Error(lexeme, info->line_no));
    }
    
    std::vector<Error *> *SPL_Driver::get_errors() {
        if (scanner->lexical_errors->empty()) {
            return syntax_errors;
        } else if (syntax_errors->empty()) {
            return scanner->lexical_errors;
        }

        std::vector<Error *> *all = new std::vector<Error *>(scanner->lexical_errors->begin(),scanner->lexical_errors->end());
        all->insert(all->end(), syntax_errors->begin(), syntax_errors->end());
        std::sort(all->begin(), all->end(),[](Error *a, Error *b) { return a->info->line_no < b->info->line_no; });

        return all;
    }
    
    void SPL_Driver::print_errors() {
        for (const auto &error: *get_errors()) {
            std::cout << error->to_string() << std::endl;
        }
    }
}
