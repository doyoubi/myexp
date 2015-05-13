#include <string>

#include "lexer.h"
#include "test.h"

using std::string;
using std::vector;
using namespace dyb;

struct LexerTester
{
    CodeFile codeFile;
    vector<CodeLine>::iterator lineIterator;
    vector<CodeToken>::iterator tokenIterator;

    LexerTester(const string & code)
        : codeFile(CodeFile::Parse(code))
    {
    }

    void FirstLine(size_t count, const string & file, size_t line);
    void NextLine(const string & file, size_t line);
    void LastLine(const string & file, size_t line);
    void FirstToken(size_t count, const string & file, size_t line);
    void Token(size_t row, size_t column, const string & value, CodeTokenType type, const string & file, size_t line);
    void LastToken(const string & file, size_t line);
};

void LexerTester::FirstLine(size_t count, const string & file, size_t line)
{
    test_assert(codeFile.lines.size() == count, file, line);
    lineIterator = codeFile.lines.begin();
}
void LexerTester::NextLine(const string & file, size_t line)
{
    lineIterator++;
}
void LexerTester::LastLine(const string & file, size_t line)
{
    lineIterator++;
    test_assert(lineIterator == codeFile.lines.end(), file, line);
}
void LexerTester::FirstToken(size_t count, const string & file, size_t line)
{
    auto & codeLine = *lineIterator;
    test_assert(codeLine.tokens.size() == count, file, line);
    tokenIterator = codeLine.tokens.begin();
}
void LexerTester::Token(size_t row, size_t column, const string & value, CodeTokenType type,
    const string & file, size_t line)
{
    test_assert(tokenIterator != lineIterator->tokens.end(), file, line);
    test_assert(tokenIterator->row == row, file, line);
    test_assert(tokenIterator->column == column, file, line);
    test_assert(tokenIterator->value == value, file, line);
    test_assert(tokenIterator->type == type, file, line);
    tokenIterator++;
}
void LexerTester::LastToken(const string & file, size_t line)
{
    test_assert(tokenIterator == lineIterator->tokens.end(), file, line);
}

#define FIRST_LINE(code, COUNT)			    LexerTester tester(code); tester.FirstLine(COUNT, __FILE__, __LINE__);
#define NEXT_LINE							tester.NextLine(__FILE__, __LINE__);
#define LAST_LINE							tester.LastLine(__FILE__, __LINE__);
#define FIRST_TOKEN(COUNT)					tester.FirstToken(COUNT, __FILE__, __LINE__);
#define TOKEN(ROW, COLUMN, VALUE, TYPE)		tester.Token(ROW, COLUMN, VALUE, TYPE, __FILE__, __LINE__);
#define LAST_TOKEN							tester.LastToken(__FILE__, __LINE__);

void testCodeFileStructure()
{
    string code =
        "1\n"
        "2+1\n"
        "3+2+1\n"
        "\n"
        "5\n";
    FIRST_LINE(code, 4);
        FIRST_TOKEN(1);
        TOKEN(1, 1, "1", CodeTokenType::Num);
        LAST_TOKEN;
    NEXT_LINE;
        FIRST_TOKEN(3);
        TOKEN(2, 1, "2", CodeTokenType::Num);
        TOKEN(2, 2, "+", CodeTokenType::Add);
        TOKEN(2, 3, "1", CodeTokenType::Num);
        LAST_TOKEN;
    NEXT_LINE;
        FIRST_TOKEN(5);
        TOKEN(3, 1, "3", CodeTokenType::Num);
        TOKEN(3, 2, "+", CodeTokenType::Add);
        TOKEN(3, 3, "2", CodeTokenType::Num);
        TOKEN(3, 4, "+", CodeTokenType::Add);
        TOKEN(3, 5, "1", CodeTokenType::Num);
        LAST_TOKEN;
    NEXT_LINE;
        FIRST_TOKEN(1);
        TOKEN(5, 1, "5", CodeTokenType::Num);
        LAST_TOKEN;
    LAST_LINE;
}

int main()
{
    testCodeFileStructure();
    return 0;
}
