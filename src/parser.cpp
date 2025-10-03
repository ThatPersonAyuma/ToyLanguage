#include "ast.h"
#include "lexer.h"
#include "parser.h"

/// Logerror* - Fungsi Helper untukerror handling
std::unique_ptr<ExprAST> LogError(const char *Str)
{
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str)
{
    LogError(Str);
    return nullptr;
}

/// numberexpr ::= number, ekspresi untuk operasi bilangan/aritmatika
static std::unique_ptr<ExprAST> ParseNumberExpr()
{
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken(); // consume the number/cur tok
    return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr()
{
    getNextToken();
    auto V = ParseExpression();
    if (!V)
        return nullptr;
    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat ')'
    return V;
}

/// Identifierexpr
///     ::= identifier
///     ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr()
{
    std::string IdName = IdentifierStr;

    getNextToken(); // eat identifier

    if (CurTok != '(') // simple variable ref, not function call so it is a vriable
        return std::make_unique<VariableExprAST>(IdName);

    // Call (function call)
    getNextToken(); // eat '('
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')')
    { // Is there any params?
        while (true)
        {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argumen list");
            getNextToken();
        }
    }

    // Eat the ')'
    getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// primary
///     ::= identifier
///     ::= numberexpr
///     ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary()
{
    switch (CurTok)
    {
    case TokenType::tok_id:
        return ParseIdentifierExpr();
    case TokenType::tok_num:
        return ParseNumberExpr();
    case '(': // parse parenthesis example printf(((((("xczcsc")))))) => identifier printf, '(', identifier "xczcsc", ')';
        return ParseParenExpr();
    default:
        return LogError("Unknown token when expecting an expression");
    }
}

/// GetTokPrecedence - Get the precedence of the pending binary operator token
static int getTokPrecedence()
{
    if (!isascii(CurTok))
        return -1;

    // make sure it's a declared binop, cause if not in the map it will raise key error
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0) // Check if the below one, preventing
        return -1;
    return TokPrec;
}

/// expression
///     ::= primary binoprhs
///
static std::unique_ptr<ExprAST> ParseExpression()
{
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

/// binoprhs
///     ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                            std::unique_ptr<ExprAST> LHS)
{
    // if this is a binop, find its precedence
    while (true)
    {
        int TokPrec = getTokPrecedence();

        // If this is a binop that binds at least as tightly as current binop,
        // consume it, otherwise we are done.
        if (TokPrec < ExprPrec)
            return LHS;

        // This is a binop for sure
        int BinOp = CurTok;
        getNextToken(); // eat binop

        // Parse the primary expression after the binary operator.
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        // If BinOp binds less tightly with RHS the operator after RHS,
        // let the pending opearator take RHS as its LHS.
        int NextPrec = getTokPrecedence();
        if (TokPrec < NextPrec)
        {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        // Merge LHS/RHS.
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
                                            std::move(RHS));
    } // Loop around t the top of the while loop
}

/// prototype
///     ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype()
{
    if (CurTok != TokenType::tok_id)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string> ArgNames;
    while (getNextToken() == TokenType::tok_id)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // success
    getNextToken(); // eat the ')'

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition()
{
    getNextToken(); // eat def.
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (auto E = ParseIdentifierExpr())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

/// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern()
{
    getNextToken(); // eat extern
    return ParsePrototype();
}

/// topLevelexpr ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr()
{
    if (auto E = ParseExpression())
    {
        // Make an anonymous proto
        auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

/* #region Top Level Parsing */
void HandleDefinition(){
    if (ParseDefinition()){
        fprintf(stderr, "Parsed a function definition.\n");
    }else{
        // Skip token for error recovery;
        getNextToken();
    }
}
void HandleExtern(){
    if (ParseExtern()){
        fprintf(stderr, "Parsed an extern\n");
    }else{
        // Skip token for error recovery.
        getNextToken();
    }
}
void HandleTopLevelExpression(){
    if (ParseTopLevelExpr()){
        fprintf(stderr, "Parsed a top-level expr\n");
    }else{
        // Skip token for error recovery.
        getNextToken();
    }
}
/* #endregion */