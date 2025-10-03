#pragma once
#include <string>
#include <fstream>
#include <map>

/// TokenType - Jenis token yang dikenali lexer
enum TokenType {
    tok_eof = -1,   // End of file
    // Command/Keyword
    tok_def = -2,
    // Primary
    tok_id  = -3,   // identifier
    tok_num = -4,   // number (float/int)
    tok_extern = -5 // keyword 'extern'
};

/// Token - Representasi sebuah token
struct Token {
    int value;         // ASCII char atau enum TokenType
    TokenType type;    // tipe token
};

// === Global state yang dipakai lexer ===
extern std::string IdentifierStr; // kalau token = tok_id
extern double NumVal;             // kalau token = tok_num
extern std::ifstream readStream;  // input stream file
extern int CurTok;                // token saat ini
extern std::map<char, int> BinopPrecedence; // Binary operation map 

/// getTok - membaca 1 token dari input
int getTok();

/// getNextToken - update CurTok dengan token baru
int getNextToken();