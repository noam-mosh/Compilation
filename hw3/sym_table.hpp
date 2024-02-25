#ifndef _236360_3_sym_table
#define _236360_3_sym_table

#include <stack>
#include "types.hpp"
#include <string>
#include <vector>
extern int yylineno;
using namespace std;

class Func_entry;
class Table_entry
{
public:
    Table_entry(string name, Type type, int offset) : name(name), type(type), offset(offset) {}
    const string name;
    Type type;
    int offset;
    virtual Func_entry *func_already_defined(string &, Type, Type) { return nullptr; };
    virtual bool func_callable(string &, Type) { return false; };
    virtual Type get_var_type(string name) { return UND_; };
    virtual void print_id() = 0;
    virtual ~Table_entry() = default;
};

class Var_entry : public Table_entry{
public:
    Var_entry(string name, Type type, int offset) : Table_entry(name, type, offset){};
    Type get_var_type(string name) override;
    void print_id() override;
};

class Func_entry : public Table_entry{
public:
    Func_entry(string name, Type type, int offset, Type arg_type):
                Table_entry(name, type, offset), arg_type(arg_type){}
    Type arg_type; 
    virtual Func_entry *func_already_defined(string &, Type, Type) override;
    virtual bool func_callable(string &, Type) override;
    Type get_var_type(string name) override;
    void print_id() override;
};


class Table{
public:
    Table(bool is_func = false) : is_func(is_func){}
    bool is_func;
    vector<Table_entry *> symbols;
    void push(Table_entry *);
    Type get_var_type(string name);
    Func_entry* func_already_defined(string &, Type, Type);
    vector<Type> get_callable_funcs(string &, Type);
    void print_ids();
};

class Symtab{
public:
    vector<Table *> tables;
    stack<int> offsets;
    int while_counter;
    Type current_return_type;

    Symtab();
    void push(string name, Type type, bool is_func = false, Type arg_type = UND_,  int offset = 0, bool offset_valid = false);
    void remove_scope();
    void create_scope();
    void start_func_scope(Type ret_type,Type arg_type, string arg_name);
    void end_func_scope();
    void start_while_scope();
    void end_while_scope();
    void assert_legal_assignment(Type dst, Type src);
    Type get_var_type(string name);
    Func_entry* func_already_defined(string &, Type, Type);
    vector<Type> get_callable_funcs(string &, Type);
    Type calc_binop_type(Type t1, Type t2);
    void assert_byte_range(int val);
    void assert_bool(Type t);
    void assert_numeric(Type t);
    void assert_return_type(Type t);
    void assert_break_in_while();
    void assert_cont_in_while();
    Type call_func(string name, Type arg);
    void assert_defined(Type type, string name);
 private:
};

#endif