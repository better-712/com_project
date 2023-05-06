#include <iostream>
#include <vector>
#include "type.cpp"
#include "ast.hpp"
#include <string.h>
namespace SPL {
  //char* cgen_Stmt(Node* tree, int indent);
  //char* cgen_CompSt(Node* tree, int indent);
  
  int INDENT_LEV=4;
  std::vector<Node *>* list_to_ele(Node *node){
        auto *decs = new std::vector<Node *>{};
        decs->push_back(node->children.front());
        Node *list=node->children.back();
        while(list->type.compare(node->type) == 0){
            decs->push_back(list->children.front());
            list=list->children.back();
        }
        return decs;
    }
  char* cgen_Int (Node* tree) {
    return (char*)(tree->value).c_str();
  }
  char* cgen_FLOAT (Node* tree) {
    return (char*)(tree->value).c_str();
  }
  char* cgen_CHAR (Node* tree) {
    return (char*)(tree->value).c_str();
  }
  char* cgen_ID(Node* tree){
    return (char*)(tree->value).c_str();
  }
  
  char* cgen_op(Node* tree){
//     if(tree->children[0]->type.compare("ASSIGN") == 0)
//       return (char*)"=";
//     if(tree->children[0]->type.compare("AND") == 0)
//       return (char*)"&&";
//     if(tree->children[0]->type.compare("OR") == 0)
//       return (char*)"||";
//     if(tree->children[0]->type.compare("LT") == 0)
//       return (char*)"<";
//     if(tree->children[0]->type.compare("LE") == 0)
//       return (char*)"<=";
//     if(tree->children[0]->type.compare("GT") == 0)
//       return (char*)">";
//     if(tree->children[0]->type.compare("GE") == 0)
//       return (char*)">=";
//     if(tree->children[0]->type.compare("NE") == 0)
//       return (char*)"!=";
//     if(tree->children[0]->type.compare("EQ") == 0)
//       return (char*)"==";
//     if(tree->children[0]->type.compare("PLUS") == 0)
//       return (char*)"+";
//     if(tree->children[0]->type.compare("MINUS") == 0)
//       return (char*)"-";
//     if(tree->children[0]->type.compare("MUL") == 0)
//       return (char*)"*";
//     if(tree->children[0]->type.compare("DIV") == 0)
//       return (char*)"/";
    return (char*)(tree->value).c_str();
  }
 
  
  char* cgen_Exp(Node* tree){
    if (tree->children[0]->type.compare("INT") == 0)
        return cgen_Int(tree->children[0]);
    if (tree->children[0]->type.compare("ID") == 0)
        return cgen_ID(tree->children[0]);
    if (tree->children[0]->type.compare("FLOAT") == 0)
        return cgen_FLOAT(tree->children[0]);
    if (tree->children[0]->type.compare("CHAR") == 0)
        return cgen_CHAR(tree->children[0]);
    if (tree->children.size()==3&&tree->children[0]->type.compare("Exp") == 0&&tree->children[2]->type.compare("Exp") == 0){
      char  *exp1,*op,*exp2,*result;
      int l_exp1,l_op,l_exp2;
      exp1=cgen_Exp(tree->children[0]);
      l_exp1=strlen(exp1);
      op=cgen_op(tree->children[1]);
      l_op=strlen(op);
      exp2=cgen_Exp(tree->children[2]);
      l_exp2=strlen(exp2);
      result = (char*)calloc(l_exp1 + l_op + l_exp2, sizeof(char));
      memcpy(result, exp1, l_exp1 * sizeof(char));
      memcpy(result+l_exp1, op, l_op * sizeof(char));
      memcpy(result+l_exp1+l_op, exp2, l_exp2 * sizeof(char));
      return result;
    }
    
    return (char*)"cgen_Exp";
  }
  
  
  char* cgen_VarDec(Node* tree){
    char  *result;
    if(tree->children.size() == 4)
    {
      char *id,*in;
      int l_id,l_in;
      id = cgen_ID(tree->children[0]);
      l_id= strlen(id);
      in=cgen_Int(tree->children[2]);
      l_in= strlen(in);
      result = (char*)calloc(l_id + 2 + l_in, sizeof(char));
      memcpy(result, id, l_id * sizeof(char));
      result[l_id] = '[';
      memcpy(result+l_id + 1, in, l_in * sizeof(char));
      result[l_id + 1 + l_in] = ']';
    }else{
      result=cgen_ID(tree->children[0]);
    }
    return result;
  }
  char* cgen_Specifier(Node* tree){
    char  *result;
    if(tree->children[0]->type.compare("TYPE")==0){
      result=(char*)(tree->children[0]->value).c_str();
    }else{
      //to do
    }
    return result;
  }
  char* cgen_ExtDecList(Node* tree){
    char *var,*extDecList, *result;
    int l_var,l_ext;
    var = extDecList = result = NULL;
    l_var = l_ext = 0;
    
    var = cgen_VarDec(tree->children[0]);
    l_var = strlen(var);
    if(tree->children.size() == 3){
      extDecList= cgen_ExtDecList(tree->children[2]);
      l_ext = strlen(extDecList);
    }
    
    if (l_ext > 0)
      result = (char*)calloc(l_var + 1 + l_ext, sizeof(char));
    else
      result = (char*)calloc(l_var, sizeof(char));
    
    memcpy(result, var, l_var * sizeof(char));
    if (l_ext > 0){
        result[l_var] = ',';
        memcpy(result + l_var + 1, extDecList, l_ext * sizeof(char));
    }
    
    return result;
  }
  char* cgen_Specifier_ExtDecList_SEMI(Node* tree, int indent){
    char *Specifier, *ExtDecList, *result;
    int l_spec, l_ext;

    Specifier = ExtDecList = result = NULL;
    l_spec = l_ext = 0;
    
    Specifier = cgen_Specifier(tree->children[0]);
    l_spec = strlen(Specifier);
    
    ExtDecList = cgen_ExtDecList(tree->children[1]);
    l_ext = strlen(ExtDecList);
    
    result = (char*)calloc(l_spec + 1 +l_ext, sizeof(char));
    memcpy(result, Specifier, l_spec * sizeof(char));
    result[l_spec] = ' ';
    memcpy(result + l_spec + 1, ExtDecList, l_ext * sizeof(char));
        
    return result;
  }
  
  char* cgen_ParamDec(Node* tree){
    char *Specifier,* VarDec, *result;
    int l_spec,l_VarDec;
    Specifier = VarDec = result = NULL;
    l_spec = l_VarDec = 0;
    Specifier = cgen_Specifier(tree->children[0]);
    l_spec = strlen(Specifier);
    VarDec = cgen_VarDec(tree->children[1]);
    l_VarDec = strlen(VarDec);
    
    result = (char*)calloc(l_spec + 1 +l_VarDec, sizeof(char));
    memcpy(result, Specifier, l_spec * sizeof(char));
    result[l_spec] = ' ';
    memcpy(result + l_spec + 1, VarDec, l_VarDec * sizeof(char));
        
    return result;
  }
  
  char* cgen_VarList(Node* tree){
    char *ParamDec,*VarList, *result;
    int l_ParamDec,l_VarList;
    ParamDec = VarList = result = NULL;
    l_ParamDec = l_VarList = 0;
    
    ParamDec = cgen_ParamDec(tree->children[0]);
    l_ParamDec = strlen(ParamDec);
    if(tree->children.size() == 3){
      VarList= cgen_VarList(tree->children[2]);
      l_VarList = strlen(VarList);
    }
    
    if (l_VarList > 0)
      result = (char*)calloc(l_ParamDec + 1 + l_VarList, sizeof(char));
    else
      result = (char*)calloc(l_ParamDec, sizeof(char));
    
    memcpy(result, ParamDec, l_ParamDec * sizeof(char));
    if (l_VarList > 0){
        result[l_ParamDec] = ',';
        memcpy(result + l_ParamDec + 1, VarList, l_VarList * sizeof(char));
    }
    
    return result;
  }
  
  char* cgen_FunDec(Node* tree){
    char *id,*VarList,*result;
    int l_id,l_VarList;
    id=cgen_ID(tree->children[0]);
    l_id=strlen(id);
    if(tree->children.size() == 4){
      VarList=cgen_VarList(tree->children[2]);
      l_VarList=strlen(VarList);
    }
    result = (char*)calloc(l_id + 2 +l_VarList, sizeof(char));
    memcpy(result, id, l_id * sizeof(char));
    result[l_id] = '(';
    if(l_VarList!=0)memcpy(result+l_id+1, VarList, l_VarList * sizeof(char));
    result[l_id+l_VarList+1] = ')';
    return result;
  }
  
  char* cgen_Dec(Node* tree, int indent){
    //indent
    char *result;
    result=NULL;
    if(tree->children.size()==1){
      char *var;
      int l_var;
      var=NULL;
      l_var=0;
      
      var = cgen_VarDec(tree->children[0]);
      l_var = strlen(var);
      result = (char*)calloc(l_var+1, sizeof(char));
      memcpy(result, var, l_var * sizeof(char));
      result[l_var]='\n';
    }
    else{
      char *var,*exp;
      int l_var,l_exp;
      var=exp=NULL;
      l_var=l_exp=0;
      var = cgen_VarDec(tree->children[0]);
      l_var = strlen(var);
      exp = cgen_Exp(tree->children[2]);
      l_exp = strlen(exp);
      result = (char*)calloc(l_var+l_exp+2, sizeof(char));
      memcpy(result, var, l_var * sizeof(char));
      result[l_var]='=';
      memcpy(result+l_var+1, exp, l_exp * sizeof(char));
      result[l_var+1+l_exp]='\n';
    }
    return result;
  }
  
  char* cgen_DecList(Node* tree, int indent){
    char *Dec,*DecList, *result;
    int l_Dec,l_DecList;
    Dec = DecList = result = NULL;
    l_Dec = l_DecList = 0;
    
    Dec = cgen_Dec(tree->children[0],indent);
    l_Dec = strlen(Dec);
    if(tree->children.size() == 3){
      DecList= cgen_DecList(tree->children[2],indent);
      l_DecList = strlen(DecList);
    }
    
    if (l_DecList > 0)
      result = (char*)calloc(l_Dec + 1 + l_DecList, sizeof(char));
    else
      result = (char*)calloc(l_Dec, sizeof(char));
    
    memcpy(result, Dec, l_Dec * sizeof(char));
    if (l_DecList > 0){
        result[l_Dec] = ',';
        memcpy(result + l_Dec + 1, DecList, l_DecList * sizeof(char));
    }
    
    return result;
  }
  
  char* cgen_Def(Node* tree, int indent){
    //indent
    char *Specifier, *DecList, *result;
    int l_spec, l_DecList;

    Specifier = DecList = result = NULL;
    l_spec = l_DecList = 0;
    
    Specifier = cgen_Specifier(tree->children[0]);
    l_spec = strlen(Specifier);
    
    DecList = cgen_DecList(tree->children[1],indent);
    l_DecList = strlen(DecList);
    
    
    result = (char*)calloc(indent+l_spec + 1 +l_DecList, sizeof(char));
    memset(result, ' ', indent * sizeof(char));
    memcpy(result+indent, Specifier, l_spec * sizeof(char));
    result[indent+l_spec] = ' ';
    memcpy(result +indent+ l_spec + 1, DecList, l_DecList * sizeof(char));
        
    return result;
  }
  
  char* cgen_DefList(Node* tree, int indent){
    //null? ""!
    if(tree->type.compare("empty") == 0)return (char*)"";
    char *Def,*DefList, *result;
    int l_Def,l_DefList;
    Def = DefList = result = NULL;
    l_Def = l_DefList = 0;
    
    Def = cgen_Def(tree->children[0],indent);
    l_Def = strlen(Def);
    DefList= cgen_DefList(tree->children[1],indent);
    l_DefList = strlen(DefList);
    
    result = (char*)calloc(l_Def + l_DefList, sizeof(char));
    memcpy(result, Def, l_Def * sizeof(char));
    memcpy(result+l_Def, DefList, l_DefList * sizeof(char));
    return result;
  }
  
  char* cgen_Stmt(Node* tree, int indent){
    if(tree->children[0]->type.compare("Exp") == 0){
       char * result;
       char * exp=cgen_Exp(tree->children[0]);
       int l_exp=strlen(exp);
       result = (char*)calloc(indent+l_exp, sizeof(char));
       memset(result, ' ', indent * sizeof(char));
       memcpy(result+indent, exp, l_exp * sizeof(char));
       return result;
     }
    if(tree->children[0]->type.compare("CompSt") == 0)
       return cgen_CompSt(tree->children[0],indent);
    
    //Exp SEMI
    //CompSt
    //RETURN Exp SEMI
    //IF LP Exp RP Stmt
    //IF LP Exp RP Stmt ELSE Stmt
    //WHILE LP Exp RP Stmt
    
    return (char*)"cgen_Stmt";
  }
  
  char* cgen_StmtList(Node* tree, int indent){
    //Stmt StmtList
    if(tree->type.compare("empty") == 0)return (char*)"";
    char *Stmt,*StmtList, *result;
    int l_Stmt,l_StmtList;
    Stmt = StmtList = result = NULL;
    l_Stmt = l_StmtList = 0;
    
    Stmt = cgen_Stmt(tree->children[0],indent);
    l_Stmt = strlen(Stmt);
    StmtList= cgen_StmtList(tree->children[1],indent);
    l_StmtList = strlen(StmtList);
    
    result = (char*)calloc(l_Stmt + l_StmtList, sizeof(char));
    memcpy(result, Stmt, l_Stmt * sizeof(char));
    memcpy(result+l_Stmt, StmtList, l_StmtList * sizeof(char));
    return result;
  }
  
  char* cgen_CompSt(Node* tree, int indent){
    char *DefList,*StmtList,*result;
    int l_DefList,l_StmtList;
    DefList=cgen_DefList(tree->children[1],indent);
    l_DefList=strlen(DefList);
    StmtList=cgen_StmtList(tree->children[2],indent);
    l_StmtList=strlen(StmtList);
    
    result = (char*)calloc(l_DefList + 3 +l_StmtList, sizeof(char));
    result[0] = '{';
    result[1] = '\n';
    memcpy(result+2, DefList, l_DefList * sizeof(char));
    memcpy(result+2+l_DefList, StmtList, l_StmtList * sizeof(char));
    result[l_DefList+2+l_StmtList] = '}';
    
    return result;
  }
  
  char* cgen_Specifier_FunDec_CompSt(Node* tree, int indent){
    char *Specifier, *FunDec,*CompSt, *result;
    int l_spec, l_fun, l_comp;

    Specifier = FunDec =CompSt= result = NULL;
    l_spec = l_fun =l_comp= 0;
    
    Specifier = cgen_Specifier(tree->children[0]);
    l_spec = strlen(Specifier);
    
    FunDec = cgen_FunDec(tree->children[1]);
    l_fun = strlen(FunDec);
    
    CompSt = cgen_CompSt(tree->children[2],indent+INDENT_LEV);
    l_comp = strlen(CompSt);
    
    result = (char*)calloc(l_spec + l_fun +l_comp+1, sizeof(char));
    memcpy(result, Specifier, l_spec * sizeof(char));
    result[l_spec] = ' ';
    memcpy(result+l_spec+1, FunDec, l_fun * sizeof(char));
    memcpy(result+l_spec+1+l_fun, CompSt, l_comp * sizeof(char));
    return result;
  }
  

  char* cgen_ExtDef (Node* tree, int indent){
    printf("cgen_Line\n");
    
    
    char *line, *result;
    int l_line;
      
    Node *tmp=tree->children[1];
    if(tmp->type.compare("ExtDecList") == 0)
      line = cgen_Specifier_ExtDecList_SEMI(tree, indent);
    else if(tmp->type.compare("SEMI") == 0)
      printf("SEMI\n");
    else if(tmp->type.compare("FunDec") == 0)
      line = cgen_Specifier_FunDec_CompSt(tree, indent);
    
    l_line = strlen(line);
    result = (char*)calloc(l_line + 2, sizeof(char));
    memcpy(result, line, l_line * sizeof(char));
    // Add the ENDLINE TOKEN
    memset(result + l_line, '\n', sizeof(char));
    
    return result;
  }
  char* cgen_Program (Node* tree, int indent){
    char *result;
    int nLines, total, last, l_line;
    Node *ExtDefList=tree->children[0];

    result = NULL;
    nLines = last = total = l_line = 0;
    std::vector<Node *>* list=list_to_ele(ExtDefList);
    
    nLines=list->size();
    char* lines[nLines];
    nLines = 0;
    for(auto iter=list->begin();iter!=list->end();iter++){
        lines[nLines] = cgen_ExtDef((*iter), indent);
        printf("line is %s\n", lines[nLines]);
        if (lines[nLines] == NULL)
            printf("lines[%d] == NULL\n",nLines);
        total += strlen(lines[nLines]);
        nLines++;
    }
    result = (char*)calloc(total + 1, sizeof(char));
    
    for (int i=0; i<nLines; i++) {
        l_line = strlen(lines[i]);
        memcpy(result + last, lines[i], l_line * sizeof(char));
        last += l_line;
        //free(lines[i]);
    }
    printf("result is %s\n", result);
    return result;
    
  }
  
  char* code_gen (Node *root){
    printf("code_gen\n");
    return cgen_Program(root, 0);
  }
  
  
}
