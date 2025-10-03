#include <iostream>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream> // Include header file fstream
#include "parser.h"
using namespace std;
/// top ::= definition | external | expression | ';'
static void MainLoop(){
    while(true){
        switch (CurTok){
            case TokenType::tok_eof:
                return;
            case ';':
                getNextToken();
                break;
            case TokenType::tok_def:
                HandleDefinition();
                break;
            case TokenType::tok_extern:
                HandleExtern();
                break;
                
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}
/* #endregion */

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Please Include file path" << endl;
        return 1;
    }
    if (!filesystem::exists(argv[1]))
    {
        cerr << "Path didn't exist" << endl;
        return 1;
    }
    // cout << "Number of argument: " << argc << endl;
    // for (int i = 0; i < argc; ++i)
    // {
    //     // You can convert char* to std::string for easier manipulation
    //     std::string arg_str = argv[i];
    //     std::cout << "Argument " << i << ": " << arg_str << std::endl;
    // } // Buat objekofstream
    // define BinopPredence value
    // 1 is lowest precedence
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 30;
    BinopPrecedence['*'] = 40; // highest, only range so the value can be adjust
    readStream.open(argv[1]);  // Buka file dengan fungsi open()

    printf("Start\n");
    getNextToken();
    MainLoop();
    // if (readStream.is_open())
    // {
        // while (getline(myFile, line))
        // {
        //     cout << line << endl;
        // }
        // myFile.close();
        // while (true)
        // {
        //     int tok = getTok();
        //     if (tok == TokenType::tok_eof)
        //     {
        //         break;
        //     }
        //     switch (tok)
        //     {
        //     case TokenType::tok_id:
        //         cout << "Token Id: " << IdentifierStr << " ";
        //         break;
        //     case TokenType::tok_num:
        //         cout << "Num Value: " << NumVal << " ";
        //         break;
        //     default:
        //         break;
        //     }
        //     cout << tok << " ";
        // }
    // }

    return 0; // Indicate successful execution
}
