#ifndef DYB_COMPILE_ERROR
#define DYB_COMPILE_ERROR

#include <string>

namespace dyb
{
    enum class CompileErrorType
    {
        Lexer_UnexpectedChar,
        Parser_ExpectNumButOpFound,
        Parser_ExpectOpButNumFound,
        Parser_UnKnownTokenFound,
        Parser_NoMoreToken,
        Parser_LineNotFullyParsed,
        Parser_ExpectAddButMulFound,
        Parser_ExpectMulButAddFound,
    };

}

#endif
