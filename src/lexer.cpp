#include "lexer.h"
using namespace std;
/* #region Definition */
// token has Ascii value = [0-255] if it known, if not we'll use token below
// === Definisi Global Variables ===
std::string IdentifierStr;
double NumVal;
std::ifstream readStream;
int CurTok;
std::map<char, int> BinopPrecedence;
/* #region  function*/
int getNextToken()
{
    return CurTok = getTok();
}
int getTok()
{
    static int lastChar = ' ';

    while (isspace(lastChar))
    { // Skip any white space
        lastChar = readStream.get();
    }
    if (isalpha(lastChar))
    { // identifier: index pertama [a-zA-Z], sisanya [a-zA-Z0-9]
        IdentifierStr = lastChar;
        while (isalnum((lastChar = readStream.get())))
        {
            IdentifierStr += lastChar;
        }
        if (IdentifierStr == "def")
        {
            return TokenType::tok_def;
        }
        if (IdentifierStr == "extern")
            return TokenType::tok_extern;
        return TokenType::tok_id;
    }
    if (isdigit(lastChar))
    { // Number [0-9]
        string NumStr;
        do
        {
            NumStr += lastChar;
            lastChar = readStream.get();
        } while (isdigit(lastChar) || lastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return TokenType::tok_num;
    }
    if (lastChar == EOF)
    {
        return TokenType::tok_eof;
    }
    int resultChar = lastChar;
    lastChar = readStream.get();
    return resultChar;
}

