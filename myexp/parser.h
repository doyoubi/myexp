#ifndef DYB_PARSER
#define DYB_PARSER

#include <vector>
#include <initializer_list>
#include <memory>

#include "lexer.h"

namespace dyb
{

    enum class OperatorType
    {
        Add,
        Mul,
    };

    template<typename T>
    using AstNodePtr = std::shared_ptr<T>;

    struct AstNode
    {
        typedef AstNodePtr<AstNode> Ptr;

        virtual ~AstNode() {}
        std::vector<AstNode::Ptr> children;
    };

    struct AstNonTerminalNode : public AstNode
    {
        typedef AstNodePtr<AstNonTerminalNode> Ptr;

        static AstNode::Ptr genNonTerminalNode(std::initializer_list<AstNode::Ptr> children);
    };

    struct AstTerminalNode : public AstNode
    {
        typedef AstNodePtr<AstTerminalNode> Ptr;
    };

    struct AstOperand : public AstTerminalNode
    {
        typedef AstNodePtr<AstOperand> Ptr;

        AstOperand(int num) : number(num) {}
        int number;
    };

    struct AstOperator : public AstTerminalNode
    {
        typedef AstNodePtr<AstOperator> Ptr;

        AstOperator(OperatorType _type) : type(_type) {}
        OperatorType type;
    };

    struct ParserError
    {
        typedef std::vector<ParserError> List;

        CompileErrorType errorType;
        CodeToken token;
        std::string errorMsg;
    };

    struct ExpressionParser
    {
        typedef CodeToken::List::const_iterator TokenIter;

        static AstNode::Ptr ParseLine(CodeLine line);
        static AstNode::Ptr ParseTerm(CodeTokenType tokenType);
        static AstNode::Ptr ParseFactor();
        static AstNode::Ptr ParseExp();

        static ParserError::List errors;

    private:
        static TokenIter tokenIter;
        static TokenIter lineEnd;
    };

}

#endif
