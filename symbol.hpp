#include <vector>
#include <unordered_map>

#include "ast.hpp"

namespace SPL {
  using namespace std;
  class Symbol_Table;
  class Symbol;
  
  struct Symbol_Type{
    int type=-1;
    //only for struct
    string tag="tag";
    int size=-1;
    int is_fun=0;
    int value=-10000;
    vector<Symbol*> parm_type;
    vector<Symbol*> arg_type;
  };
  
  
  class Symbol {
    public:
        string name;
    //funtion return type 
        Symbol_Type symbol_type;

        int line_no = -1;
    };
  
  class Symbol_Table {
    public:
        Symbol_Table(): next(nullptr) {}

        void set_next(Symbol_Table *next);

        void insert(Symbol *entry);
    
        Symbol* find_symbol(std::string name);
    
        void print_table();
    

        Symbol_Table * next;
    
        unordered_map<string, Symbol *> table;
    
    };

}
