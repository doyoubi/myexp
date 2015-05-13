#ifndef DYB_LEXER
#define DYB_LEXER

#include <string>
#include <vector>

#include "compile_error.h"

namespace dyb
{
    enum class CodeTokenType
    {
        Add,
        Mul,
        Num,
        UnKnown,
    };

    struct CodeToken
    {
        typedef std::vector<CodeToken> List;

        int         row = -1;
        int         column = -1;
        std::string value;
        CodeTokenType   type;
    };

    struct LexerError
    {
        typedef std::vector<LexerError> List;

        CompileErrorType errorType;
        CodeToken token;
        std::string errorMsg;
    };

    struct CodeLine
    {
        typedef std::vector<CodeLine> List;

        CodeToken::List tokens;
    };

    struct CodeFile
    {
        typedef std::vector<CodeFile> List;

        CodeLine::List lines;
        LexerError::List errors;

        static CodeFile Parse(const std::string & codeString);
    };

}

#endif
