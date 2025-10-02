//ast.h
#pragma once
#include "ast.h"

/// ExprAST - Base Class For all expression node
class ExprAST {
    public:
        virtual ~ExprAST() = default;
};
/// BinaryExprAST - Expression Class untuk operasi biner
class BinaryExprAST : public ExprAST{
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
            std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};
/// ExprAST for number
class NumberExprAST : public ExprAST{
    double Val;

public:
    NumberExprAST(double Val): Val(Val) {}
};

/// ExprAST for Identifier (Id)
class VariableExprAST : public ExprAST{
    std::string Name;

public: 
    VariableExprAST(std::string Name): Name(Name){}
};

/// CallExprAST class for Function calls
class CallExprAST : public ExprAST{
    std::string Calle;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Calle,
            std::vector<std::unique_ptr<ExprAST>> Args)
        : Calle(Calle), Args(std::move(Args)){}
};

/// PrototypeAST for a function
/// Dimana ini akan capture/menyimpan nama, dan nama argumen fungsi
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &Name,
            std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {}
    const std::string &getName() const {return Name;}
};

/// FunctionAST - Class untuk definisi fungsi
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
            std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
};
// unique ptr memastikan bahwa hanya ada satu pointer ke data dan
// hanya satu unique pointer ke data tersebut (exclusive)
// maka dari itu perlu digunakan move untuk memindah kepemilikan
// karena hanya boleh ada satu unique ptr yang menunjuk pada data
// tidak boleh adata unique ptr yang menunjukkan pada data yang sama