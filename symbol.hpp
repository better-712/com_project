#include <vector>
#include <unordered_map>

#include "ast.hpp"

namespace SPL {
  using namespace std;
  
  struct Symbol_Type{
    int type;
    string tag="tag";
    vector<int> parm_type;
  };
  
  class Symbol_Table;
  
  class Symbol {
    public:
        string name;
    //funtion return type 
        int type;
    
    //for struct
        string tag;
    
    //fun or struct
        vector<int> parm_type;

        int line_no = -1;
    };
  
  class Symbol_Table {
    public:
        Symbol_Table(): next(nullptr) {}

        void set_next(Symbol_Table *next);

        void insert(Symbol *entry);
    
        Symbol* find_symbol(std::string name);
    

        Symbol_Table * next;
    
        unordered_map<string, Symbol *> table;
    
    };

}
