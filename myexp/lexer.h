#ifndef DYB_LEXER
#define DYB_LEXER

#include <string>
#include <vector>

namespace dyb
{
    enum class TokenType
    {
        ADD,
        MUL,
        NUM,
    };

    struct Token
    {
        typedef std::vector<Token> List;

        int         row = -1;
        int         column = -1;
        std::string value;
        TokenType   type;
    };

    struct LexerError
    {
        typedef std::vector<LexerError> List;

        int         row = -1;
        int         column = -1;
        std::string errorMsg;
    };

    Token::List LexicalAnalysis(const std::string & codeString, LexerError::List & errors);

}

#endif
