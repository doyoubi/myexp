#include <vector>
#include <memory>

#include "parser.h"

namespace dyb
{
    // AstNonTerminalNode
    AstNode::Ptr AstNonTerminalNode::genNonTerminalNode(std::initializer_list<AstNode::Ptr> nodeChildren)
    {
        AstNode::Ptr node = AstNode::Ptr(new AstNonTerminalNode());
        for (auto & child : nodeChildren)
        {
            node->children.push_back(child);
        }
        return node;
    }

    // ExpressionParser
    ParserError::List ExpressionParser::errors;
    ExpressionParser::TokenIter ExpressionParser::tokenIter;
    ExpressionParser::TokenIter ExpressionParser::lineEnd;

    AstNode::Ptr ExpressionParser::ParseLine(CodeLine  line)
    {
        tokenIter = line.tokens.cbegin();
        lineEnd = line.tokens.cend();
        auto p = ParseExp();
        if (tokenIter != lineEnd)
        {
            errors.push_back({
                CompileErrorType::Parser_LineNotFullyParsed,
                *tokenIter,
                "line not fully parsed, unparsed token exists"
            });
            p = nullptr;
        }
        return p;
    }

    AstNode::Ptr ExpressionParser::ParseTerm(CodeTokenType tokenType)
    {
        if (tokenIter == lineEnd)
        {
            auto token = *std::prev(tokenIter);
            errors.push_back({
                CompileErrorType::Parser_NoMoreToken,
                token,
                //*std::prev(tokenIter),  // should not do this! It will crash as a result of compiler's bug
                "expect token but no more token found"
            });
            return nullptr;
        }

        if (tokenIter->type == tokenType)
        {
            auto p = 
                tokenType == CodeTokenType::Num ? AstNode::Ptr(new AstOperand(std::stoi(tokenIter->value))) :
                tokenType == CodeTokenType::Add ? AstNode::Ptr(new AstOperator(OperatorType::Add)) :
                tokenType == CodeTokenType::Mul ? AstNode::Ptr(new AstOperator(OperatorType::Mul)) :
                nullptr;
            ++tokenIter;
            return p;
        }

        if (tokenType == CodeTokenType::UnKnown)
        {
            errors.push_back({
                CompileErrorType::Parser_UnKnownTokenFound,
                *tokenIter,
                "unexpected 'UnKnown' token found"
            });
        }
        else if (tokenType == CodeTokenType::Num
            && (tokenIter->type == CodeTokenType::Add
            || tokenIter->type == CodeTokenType::Mul))
        {
            errors.push_back({
                CompileErrorType::Parser_ExpectNumButOpFound,
                *tokenIter,
                "expected number but operator found"
            });
        }
        else if ((tokenType == CodeTokenType::Add
            || tokenType == CodeTokenType::Mul)
            && tokenIter->type == CodeTokenType::Mul)
        {
            errors.push_back({
                CompileErrorType::Parser_ExpectOpButNumFound,
                *tokenIter,
                "expected operator but number found"
            });
        }
        else if (tokenType == CodeTokenType::Add && tokenIter->type == CodeTokenType::Mul)
        {
            errors.push_back({
                CompileErrorType::Parser_ExpectAddButMulFound,
                *tokenIter,
                "expected add but mul found"
            });
        }
        else if (tokenType == CodeTokenType::Mul && tokenIter->type == CodeTokenType::Add)
        {
            errors.push_back({
                CompileErrorType::Parser_ExpectMulButAddFound,
                *tokenIter,
                "expected mul but add found"
            });
        }
        return nullptr;
    }

    AstNode::Ptr ExpressionParser::ParseFactor()
    {
        AstNode::Ptr term = ParseTerm(CodeTokenType::Num);

        while (true)
        {
            AstNode::Ptr op = ParseTerm(CodeTokenType::Mul);
            if (!op)
            {
                errors.pop_back(); // todo : amend error control
                return term;
            }
            AstNode::Ptr rhs = ParseTerm(CodeTokenType::Num);
            if (!rhs)
            {
                return term;
            }
            term = AstNonTerminalNode::genNonTerminalNode({
                std::move(term), std::move(op), std::move(rhs)
            });
        }
    }

    AstNode::Ptr ExpressionParser::ParseExp()
    {
        AstNode::Ptr term = ParseFactor();

        while (true)
        {
            AstNode::Ptr op = ParseTerm(CodeTokenType::Add);
            if (!op)
            {
                errors.pop_back();
                return term;
            }
            AstNode::Ptr rhs = ParseFactor();
            if (!rhs)
            {
                return term;
            }
            term = AstNonTerminalNode::genNonTerminalNode({
                std::move(term), std::move(op), std::move(rhs)
            });
        }
    }

}
