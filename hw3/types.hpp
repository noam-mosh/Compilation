#ifndef _236360_3_types
#define _236360_3_types

#include <vector>
#include <string>
#define YYSTYPE Attr*

using namespace std;
enum Type
{
    UND_,
    INT_,
    BOOL_,
    BYTE_,
    STRING_,
    VOID_,
    FUNC_
};

class Attr {
public:
    virtual ~Attr() {};
};

class Bool_attr : public Attr{
public:
    bool value;
    Bool_attr(bool value) : value(value) {};
};

class Type_attr : public Attr{
public:
    Type type;
    Type_attr(Type type) : type(type) {};
};


class Name_attr : public Attr{
public:
    string name;
    explicit Name_attr(string name) : name(name) {};
};

class Number_attr : public Attr{
public:
    int num;
    explicit Number_attr(string num) : num(stoi(num)) {};
};

class Type_name_attr: public Attr{
public:
    Type type;
    string name;
    Type_name_attr(Type type, string name = ""): type(type), name(name){}
};


class Op_attr: public Attr{
public:
    string op;
    Op_attr(string op): op(op) {};
};

#endif