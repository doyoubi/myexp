#include "lexer.h"

namespace dyb
{
    using std::string;

    Token::List LexicalAnalysis(const string & codeString, LexerError::List & errors)
    {
        enum class State
        {
            Begin,
            InNumber,
        };

        Token::List tokenList; // only support one row now
        int row = 1, column = 1;
        auto addToken = [&](const string value, TokenType type){
            Token token;
            token.row = row;
            token.column = column;
            token.value = value;
            token.type = type;
            tokenList.push_back(token);
        };
        auto addError = [&](const string errorMsg){
            LexerError error;
            error.row = row;
            error.column = column;
            error.errorMsg = errorMsg;
            errors.push_back(error);
        };
        State state = State::Begin;
        auto charIt = std::begin(codeString);
        auto head = std::end(codeString);

        while (true)
        {
            char c = charIt == std::end(codeString) ? '\0' : *charIt;
            switch (state)
            {
            case State::Begin:
                switch (c)
                {
                case '\0':
                case ' ':
                    break;
                case '+':
                    addToken(string(1, c), TokenType::ADD);
                    break;
                case '*':
                    addToken(string(1, c), TokenType::MUL);
                    break;
                default:
                    if ('0' <= c && c <= '9')
                    {
                        head = charIt;
                        state = State::InNumber;
                    }
                    else
                    {
                        addError("illegal char found: '" + string(1, c) + "'");
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
                else if (c == ' ' || c == '\n' || c == '\0')
                {
                    addToken(string(head, charIt), TokenType::NUM);
                    state = State::Begin;
                    head = std::end(codeString);
                }
                else
                {
                    addError("expected digit but '" + string(1, c) + "' found");
                    // ignore this char and go on from State::Begin
                    state = State::Begin;
                }
                break;
            } // end of switch
            if (charIt == std::end(codeString)) break;
            ++charIt;
            ++column;
        }

        return tokenList;
    }
}
