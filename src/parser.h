#pragma once
#include "ast.h"
#include "lexer.h"

/// Logerror* - Fungsi Helper untukerror handling
std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
/// numberexpr ::= number, ekspresi untuk operasi bilangan/aritmatika
static std::unique_ptr<ExprAST> ParseNumberExpr();
/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr();
/// Identifierexpr
///     ::= identifier
///     ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr();
/// primary
///     ::= identifier
///     ::= numberexpr
///     ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary();
/// GetTokPrecedence - Get the precedence of the pending binary operator token
static int getTokPrecedence();
/// expression
///     ::= primary binoprhs
///
static std::unique_ptr<ExprAST> ParseExpression();

/// binoprhs
///     ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                            std::unique_ptr<ExprAST> LHS);
/// prototype
///     ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype();

/// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition();
/// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern();

/// topLevelexpr ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr();

/* #region Top Level Parsing */
void HandleDefinition();
void HandleExtern();
void HandleTopLevelExpression();
/* #endregion */