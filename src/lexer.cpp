#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream> // Include header file fstream
#include <string>
#include <vector>
using namespace std;
/* #region Definition */
// token has Ascii value = [0-255] if it known, if not we'll use token below 
enum TokenType
{
    tok_eof = -1, // end of file
    // Command/Keyword
    tok_def = -2,
    // Primary
    tok_id = -3, // id
    tok_num = -4,     // Float and integer
    // OpenParen,
    // CloseParen,
    // Operand,
    // Eof // end of file
};
struct Token
{
    int value;
    TokenType type;
};

/* #endregion */
static string IdentifierStr="";
static double NumVal;
static int GetTok(ifstream &readStream){
    static int lastChar = ' ';

    while(isspace(lastChar)){// Skip any white space
        lastChar = readStream.get();
    }
    if (isalpha(lastChar)){ // identifier: index pertama [a-zA-Z], sisanya [a-zA-Z0-9]
        IdentifierStr = lastChar;
        while(isalnum((lastChar = readStream.get()))){
            IdentifierStr+=lastChar;
        }
        if (IdentifierStr == "def"){
            return TokenType::tok_def;
        }
        return TokenType::tok_id;
    }
    if (isdigit(lastChar)){ // Number [0-9]
        string NumStr;
        do{
            NumStr+=lastChar;
            lastChar = readStream.get();
        }while(isdigit(lastChar) || lastChar=='.');

        NumVal = strtod(NumStr.c_str(), 0);
        return TokenType::tok_num;
    }
    if(lastChar == EOF){
        return TokenType::tok_eof;
    }
    int resultChar = lastChar;
    lastChar = readStream.get();
    return resultChar;
}


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
    cout << "Number of argument: " << argc << endl;
    for (int i = 0; i < argc; ++i)
    {
        // You can convert char* to std::string for easier manipulation
        std::string arg_str = argv[i];
        std::cout << "Argument " << i << ": " << arg_str << std::endl;
    }
    ifstream myFile;      // Buat objekofstream
    myFile.open(argv[1]); // Buka file dengan fungsi open()
    string line;
    vector<Token> tokens;
    if (myFile.is_open())
    {
        // while (getline(myFile, line))
        // {
        //     cout << line << endl;
        // }
        // myFile.close();
        while(true){
            int tok = GetTok(myFile);
            if (tok == EOF){
                break;
            }
            switch (tok)
            {
            case TokenType::tok_id :
                cout << "Token Id: " << IdentifierStr << " ";
                break;
            case TokenType::tok_num :
                cout << "Num Value: " << NumVal << " ";
                break;
            default:
                break;
            }
            cout << tok << " ";
        }
    }

    return 0; // Indicate successful execution
}
