#include "sym_table.hpp"
#include "hw3_output.hpp"
#include <iostream>
extern int yylineno;
using namespace output;

static bool type_match(Type dst, Type src){
        return (src == dst || (src == BYTE_ && dst == INT_));
}


static string TypeToString(Type type){
    static string type_to_string[] = {"UND", "INT", "BOOL", "BYTE", "STRING", "VOID"};
    string res;
    res = type_to_string[type];
    return res;
}

//TODO: What to do if args types are different order?
Func_entry* Func_entry::func_already_defined(string&name, Type type, Type arg_type){
       if(name != this->name)
        return nullptr;
            if (this->name == name && this->arg_type == arg_type && this->type == type)
            return this;
        return nullptr;
}

bool Func_entry::func_callable(string & name, Type arg_type){
    if (name != this->name)
        return false;
    if (!type_match(this->arg_type, arg_type))
        return false;
    return true;
}

void Func_entry::print_id(){
    string type_as_str = TypeToString(arg_type);
    printID(name, 0,
        makeFunctionType(type_as_str, TypeToString({type})));
}

Type Func_entry::get_var_type(string name){
    if(name == this->name){
        return FUNC_;
        // errorDef(yylineno, name);
        // exit(1);
    }
    return UND_;
}
Type Var_entry::get_var_type(string name){
    if (this->name == name)
        return this->type;
    return UND_;
}

void Var_entry::print_id(){
    printID(name, offset, TypeToString({type}));
}

Type Table::get_var_type(string name){
    for(Table_entry* entry : symbols){
        Type tmp = entry->get_var_type(name);
        if (tmp != UND_)
            return tmp;
    }
    return UND_;
}

void Table::push(Table_entry* entry){
    symbols.push_back(entry);
}

Func_entry* Table::func_already_defined(string & name, Type type, Type arg_type){
    for(Table_entry* entry : symbols){
        Func_entry* ent = entry->func_already_defined(name, type, arg_type);
        if (ent)
            return ent;
    }
    return nullptr;
}

vector<Type> Table::get_callable_funcs(string & name, Type arg_type){
    vector<Type> res;
    for(Table_entry* entry : symbols){
        if (entry->func_callable(name, arg_type))
            res.push_back(entry->type);
    }

    return res;
}

void Table::print_ids(){
    for(Table_entry* ent : symbols){
        ent->print_id();
    }
}

Symtab::Symtab(){
    offsets.push(0);
    this->create_scope();
    this->push("print", VOID_, true, {STRING_});
    this->push("printi", VOID_, true, {INT_});
    this->push("readi", INT_, true, {INT_});  
    while_counter = 0;
    current_return_type = UND_;
}

void Symtab::push(string name, Type type, bool is_func, 
                Type arg_type, int offset, bool offset_valid)
{
        // cout << "pushing " << name << endl;
        Table_entry *new_entry;
        if (!is_func){
            if (this->get_var_type(name) != UND_){
                errorDef(yylineno, name);
                exit(1);
            }
            // cout << "creatin new entry " << endl;
            if (offset_valid)
                new_entry = new Var_entry(name, type, offset);
            else
                new_entry = new Var_entry(name, type, offsets.top()++);
        }
        else{
            if (this->func_already_defined(name, type, arg_type)){
                                    errorDef(yylineno, name);
                    exit(1);
                }
                new_entry = new Func_entry(name, type, offsets.top(), arg_type);
        }
        // cout << new_entry->name << endl;
        tables.back()->push(new_entry);
}
void Symtab::remove_scope(){
    if(tables.back()->is_func){
        current_return_type = UND_;
    }
    endScope();
    tables.back()->print_ids();
    tables.pop_back();
    offsets.pop();
}

void Symtab::create_scope(){
    int at_top = offsets.top();
    offsets.push(at_top);
// cout << "CREATE: " <<  yylineno << endl;
    tables.push_back(new Table());
}

void Symtab::start_func_scope(Type ret_type, Type arg_type, string arg_name){
    create_scope();
    this->current_return_type = ret_type;
    push(arg_name, arg_type, false, {}, - 1, true);
}
void Symtab::end_func_scope(){
    this->current_return_type = UND_;
    remove_scope();
}
void Symtab::start_while_scope(){
    while_counter++;
    create_scope();
}

void Symtab::end_while_scope(){
    while_counter--;
    remove_scope();
}

void Symtab::assert_legal_assignment(Type dst, Type src){
    if (!type_match(dst, src)){
        cout << "mismatch 1" << endl;
        errorMismatch(yylineno);
        exit(1);
    }
}

void Symtab::assert_return_type(Type t){
    if(this->current_return_type == UND_){
        errorSyn(yylineno);
        exit(1);
    }
    if(!type_match(this->current_return_type, t)){
        cout << "mismatch 2" << endl;
        errorMismatch(yylineno);
        exit(1);
    }
}

Type Symtab::get_var_type(string name){
    for (Table *table : this->tables)
        {
        // table->print_ids();

        Type tmp = table->get_var_type(name);
        if (tmp != UND_)
        {
                return tmp;
            }
        }
    return UND_;
}

Func_entry* Symtab::func_already_defined(string & name, Type type, Type arg_type){
    for(Table *table : this->tables){
        Func_entry* ent = table->func_already_defined(name, type, arg_type);
        if (ent)
            return ent;
    }
    return nullptr;
}

vector<Type> Symtab::get_callable_funcs(string & name,Type arg_type){
    vector<Type> res;
    for(Table *table : this->tables){
        vector<Type> table_res =  table->get_callable_funcs(name, arg_type);
        res.insert(res.end(), table_res.begin(), table_res.end());
    }
    return res;
}

Type Symtab::calc_binop_type(Type t1, Type t2){
    if (t1 == BYTE_ && t2 == BYTE_)
        return BYTE_;
    if ((t1 == INT_ && t2 == BYTE_) || (t1 == BYTE_ && t2 == INT_) || (t1 == INT_ && t2 == INT_))
        return INT_;
    cout << "mismatch 3" << endl;
    errorMismatch(yylineno);
    exit(1);
}

void Symtab::assert_byte_range(int val){
    if (val > 255){
        errorByteTooLarge(yylineno, to_string(val));
        exit(1);
    }
}

void Symtab::assert_bool(Type t){
    if (t != BOOL_){
        cout << "mismatch 4" << endl;
        errorMismatch(yylineno);
        exit(1);
    }
}

void Symtab::assert_numeric(Type t){
    if (!((t == INT_) || (t == BYTE_))){
        cout << "mismatch 5" << endl;
        errorMismatch(yylineno);
        exit(1);
    }
}

void Symtab::assert_break_in_while(){
    if(this->while_counter <= 0){
        errorUnexpectedBreak(yylineno);
        exit(1);
    }
}
void Symtab::assert_cont_in_while(){
    if(this->while_counter <= 0){
        errorUnexpectedContinue(yylineno);
        exit(1);
    }
}

Type Symtab::call_func(string name, Type arg){
    // cout << "call func: " << yylineno << " " << name << " " << arg << endl;
    vector<Type> funcs = get_callable_funcs(name, arg);
// vector<string> types = TypeToString(args);
    if(funcs.size() == 0){
        cout << "size 0" << endl;
        if(func_already_defined(name, UND_, arg)){
            cout << "errorPrototypeMismatch " << endl;
            errorPrototypeMismatch(yylineno, name, "STRING");   //TODO: types[0]
            exit(1);
        }
        cout << "errorUndefFunc " << endl;
        errorUndefFunc(yylineno, name);
        exit(1);
    }
    // cout << "returning from call: " << funcs[0] << endl;
    return funcs[0];
}
void Symtab::assert_defined(Type type, string name){
    if(type == UND_ || type == FUNC_){
        errorUndef(yylineno, name);
        exit(1);
    }
}


 