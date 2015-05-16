#include <string>

#include "test.h"
#include "parser.h"

using std::string;
using namespace dyb;

template<class NodeType>
typename NodeType::Ptr getChild(AstNode::Ptr & parent, size_t childIndex, const string & file, size_t line)
{
    test_assert(parent->children.size() > childIndex, file, line);
    NodeType::Ptr child = std::dynamic_pointer_cast<NodeType>(parent->children[childIndex]);
    test_assert(child != nullptr, file, line);
    return child;
}
#define GETCHILD(Type, parent, childIndex) \
    getChild<Type>(parent, childIndex, __FILE__, __LINE__)

#define CHILDNUM(node, num) \
    test_assert(node->children.size() == num, __FILE__, __LINE__)


void testPrimitiveNumber()
{
    string code = "12";
    CodeFile codeFile = CodeFile::Parse(code);
    TEST_ASSERT(codeFile.lines.size() == 1);
    AstNode::Ptr root = ExpressionParser::ParseLine(codeFile.lines.front());
    TEST_ASSERT(root != nullptr);
    TEST_ASSERT(ExpressionParser::errors.size() == 0);

    auto exp = std::dynamic_pointer_cast<AstOperand>(root);
    TEST_ASSERT(exp != nullptr);
    CHILDNUM(exp, 0);
    TEST_ASSERT(exp->number == 12);
}

void testOperator(string opStr, OperatorType OpType)
{
    string code = "1" + opStr + "2";
    CodeFile codeFile = CodeFile::Parse(code);
    TEST_ASSERT(codeFile.lines.size() == 1);
    AstNode::Ptr root = ExpressionParser::ParseLine(codeFile.lines.front());
    TEST_ASSERT(root != nullptr);
    TEST_ASSERT(ExpressionParser::errors.size() == 0);
    CHILDNUM(root, 3);

    auto lhs = GETCHILD(AstOperand, root, 0);
    CHILDNUM(lhs, 0);
    TEST_ASSERT(lhs->number == 1);

    auto op = GETCHILD(AstOperator, root, 1);
    CHILDNUM(op, 0);
    TEST_ASSERT(op->type == OpType);

    auto rhs = GETCHILD(AstOperand, root, 2);
    CHILDNUM(rhs, 0);
    TEST_ASSERT(rhs->number == 2);
}

void testLeftAssociative(string opStr, OperatorType OpType)
{
    string code = "1" + opStr + "2" + opStr + "3";
    CodeFile codeFile = CodeFile::Parse(code);
    TEST_ASSERT(codeFile.lines.size() == 1);
    AstNode::Ptr root = ExpressionParser::ParseLine(codeFile.lines.front());
    TEST_ASSERT(root != nullptr);
    TEST_ASSERT(ExpressionParser::errors.size() == 0);
    CHILDNUM(root, 3);

    auto leftRoot = GETCHILD(AstNode, root, 0);
    CHILDNUM(leftRoot, 3);

    auto secondAdd = GETCHILD(AstOperator, root, 1);
    CHILDNUM(secondAdd, 0);
    TEST_ASSERT(secondAdd->type == OpType);

    auto three = GETCHILD(AstOperand, root, 2);
    CHILDNUM(three, 0);
    TEST_ASSERT(three->number == 3);

    auto one = GETCHILD(AstOperand, leftRoot, 0);
    CHILDNUM(one, 0);
    TEST_ASSERT(one->number == 1);

    auto firstAdd = GETCHILD(AstOperator, leftRoot, 1);
    CHILDNUM(firstAdd, 0);
    TEST_ASSERT(firstAdd->type == OpType);

    auto two = GETCHILD(AstOperand, leftRoot, 2);
    CHILDNUM(two, 0);
    TEST_ASSERT(two->number == 2);
}

void testOperatorOrder()
{
    string code = "1+2*3";
    CodeFile codeFile = CodeFile::Parse(code);
    TEST_ASSERT(codeFile.lines.size() == 1);
    AstNode::Ptr root = ExpressionParser::ParseLine(codeFile.lines.front());
    TEST_ASSERT(root != nullptr);
    TEST_ASSERT(ExpressionParser::errors.size() == 0);
    CHILDNUM(root, 3);

    auto one = GETCHILD(AstOperand, root, 0);
    CHILDNUM(one, 0);
    TEST_ASSERT(one->number == 1);

    auto add = GETCHILD(AstOperator, root, 1);
    CHILDNUM(add, 0);
    TEST_ASSERT(add->type == OperatorType::Add);

    auto rightRoot = GETCHILD(AstNode, root, 2);
    CHILDNUM(rightRoot, 3);

    auto two = GETCHILD(AstOperand, rightRoot, 0);
    CHILDNUM(two, 0);
    TEST_ASSERT(two->number = 2);

    auto mul = GETCHILD(AstOperator, rightRoot, 1);
    CHILDNUM(mul, 0);
    TEST_ASSERT(mul->type == OperatorType::Mul);

    auto three = GETCHILD(AstOperand, rightRoot, 2);
    CHILDNUM(three, 0);
    TEST_ASSERT(three->number == 3);
}


struct InvokeParserTest
{
    InvokeParserTest()
    {
        testPrimitiveNumber();
        testOperator("+", OperatorType::Add);
        testOperator("*", OperatorType::Mul);
        testLeftAssociative("+", OperatorType::Add);
        testLeftAssociative("*", OperatorType::Mul);
        testOperatorOrder();
    }
} invokeTest;
