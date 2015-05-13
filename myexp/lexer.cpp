#include "lexer.h"

namespace dyb
{
    using std::string;

    CodeFile CodeFile::Parse(const string & codeString)
    {
        CodeFile codeFile;

        enum class State
        {
            Begin,
            InNumber,
        };

        size_t row = 1;
        State state = State::Begin;
        auto charIt = std::begin(codeString);
        auto rowBegin = std::begin(codeString);
        auto head = std::end(codeString);

        auto addToken = [&](const string value, CodeTokenType type){
            auto tokenHead = head == std::end(codeString) ? charIt : head;
            CodeToken token;
            token.row = row;
            token.column = distance(rowBegin, tokenHead) + 1;
            token.value = value;
            token.type = type;
            if (row > codeFile.lines.size())
                codeFile.lines.push_back(CodeLine());
            codeFile.lines.back().tokens.push_back(token);
        };
        auto addError = [&](CompileErrorType errorType, const string value, const string errorMsg){
            auto tokenHead = head == std::end(codeString) ? charIt : head;
            CodeToken token;
            token.row = row;
            token.column = distance(rowBegin, tokenHead) + 1;
            token.value = value;
            token.type = CodeTokenType::UnKnown;
            LexerError error = {
                errorType, token, errorMsg
            };
            codeFile.errors.push_back(error);
        };

        while (true)
        {
            char c = charIt == std::end(codeString) ? '\0' : *charIt;
            switch (state)
            {
            case State::Begin:
                switch (c)
                {
                case '\n':
                    row++;
                    rowBegin = std::next(charIt);
                    break;
                case '\0':
                case ' ':
                    break;
                case '+':
                    addToken(string(1, c), CodeTokenType::Add);
                    break;
                case '*':
                    addToken(string(1, c), CodeTokenType::Mul);
                    break;
                default:
                    if ('0' <= c && c <= '9')
                    {
                        head = charIt;
                        state = State::InNumber;
                    }
                    else
                    {
                        addError(CompileErrorType::Lexer_UnexpectedChar, string(1, c), "illegal char found: '" + string(1, c) + "'");
                        // ignore this char and go on from State::Begin
                    }
                    break;
                }
                break;
            case State::InNumber:
                if ('0' <= c && c <= '9')
                {
                    // go on
                }
                else
                {
                    --charIt;
                    addToken(string(head, std::next(charIt)), CodeTokenType::Num);
                    state = State::Begin;
                    head = std::end(codeString);
                }
                break;
            } // end of switch
            if (charIt == std::end(codeString)) break;
            ++charIt;
        }

        return codeFile;
    }
}
