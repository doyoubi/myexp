#include "lexer.h"
#include "test.h"

using namespace dyb;

void test()
{
    LexerError::List errors;
    Token::List tokens;
    {
        tokens.clear();
        errors.clear();
        tokens = LexicalAnalysis("1", errors);
        TEST_ASSERT(tokens.size() == 1);
        TEST_ASSERT(errors.size() == 0);
        TEST_ASSERT(tokens.size() == 1 && tokens[0].type == TokenType::NUM);
        TEST_ASSERT(tokens.size() == 1 && tokens[0].value == "1");
    }
    {
        tokens.clear();
        errors.clear();
        tokens = LexicalAnalysis("+", errors);
        TEST_ASSERT(tokens.size() == 1);
        TEST_ASSERT(errors.size() == 0);
        TEST_ASSERT(tokens.size() == 1 && tokens[0].type == TokenType::ADD);
        TEST_ASSERT(tokens.size() == 1 && tokens[0].value == "+");
    }
    {
        tokens.clear();
        errors.clear();
        tokens = LexicalAnalysis("*", errors);
        TEST_ASSERT(tokens.size() == 1);
        TEST_ASSERT(errors.size() == 0);
        TEST_ASSERT(tokens.size() == 1 && tokens[0].type == TokenType::MUL);
        TEST_ASSERT(tokens.size() == 1 && tokens[0].value == "*");
    }
    {
        tokens.clear();
        errors.clear();
        tokens = LexicalAnalysis("a", errors);
        TEST_ASSERT(tokens.size() == 0);
        TEST_ASSERT(errors.size() == 1);
        TEST_ASSERT(errors.size() == 1 && errors[0].row == 1);
        TEST_ASSERT(errors.size() == 1 && errors[0].column == 1);
    }
    {
        tokens.clear();
        errors.clear();
        tokens = LexicalAnalysis("", errors);
        TEST_ASSERT(tokens.size() == 0);
        TEST_ASSERT(errors.size() == 0);
    }
}

int main()
{
    test();
    return 0;
}
