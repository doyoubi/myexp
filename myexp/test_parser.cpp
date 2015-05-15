#include <string>

#include "test.h"
#include "parser.h"

using std::string;
using namespace dyb;

void testPrimitiveNumber()
{
    string code = "12";
    CodeFile codeFile = CodeFile::Parse(code);
    TEST_ASSERT(codeFile.lines.size() == 1);
    AstNode::Ptr p = ExpressionParser::ParseLine(codeFile.lines.front());
    TEST_ASSERT(p != nullptr);
    TEST_ASSERT(ExpressionParser::errors.size() == 0);
    AstOperand::Ptr exp = std::dynamic_pointer_cast<AstOperand>(p);
    TEST_ASSERT(exp != nullptr);
    TEST_ASSERT(exp->children.size() == 0);
    TEST_ASSERT(exp->number == 12);
}

void testOperator()
{
    string code = "1+2";
    CodeFile codeFile = CodeFile::Parse(code);
    TEST_ASSERT(codeFile.lines.size() == 1);
    AstNode::Ptr p = ExpressionParser::ParseLine(codeFile.lines.front());
    TEST_ASSERT(p != nullptr);
    TEST_ASSERT(ExpressionParser::errors.size() == 0);
    AstNonTerminalNode::Ptr exp = std::dynamic_pointer_cast<AstNonTerminalNode>(p);
    TEST_ASSERT(exp != nullptr);
    TEST_ASSERT(exp->children.size() == 3);

    AstOperand::Ptr lhs = std::dynamic_pointer_cast<AstOperand>(exp->children[0]);
    TEST_ASSERT(lhs != nullptr);
    TEST_ASSERT(lhs->children.size() == 0);
    TEST_ASSERT(lhs->number == 1);

    AstOperator::Ptr op = std::dynamic_pointer_cast<AstOperator>(exp->children[1]);
    TEST_ASSERT(op != nullptr);
    TEST_ASSERT(op->children.size() == 0);
    TEST_ASSERT(op->type == OperatorType::Add);

    AstOperand::Ptr rhs = std::dynamic_pointer_cast<AstOperand>(exp->children[2]);
    TEST_ASSERT(rhs != nullptr);
    TEST_ASSERT(rhs->children.size() == 0);
    TEST_ASSERT(rhs->number == 2);
}

int main()
{
    testPrimitiveNumber();
    testOperator();
    return 0;
}
