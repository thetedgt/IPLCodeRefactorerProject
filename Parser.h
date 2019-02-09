#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <list>
#include "Lexer.h"
#include "AST.h"

struct Node;
struct ExpressionNode;
struct TypeNode;
struct ClassDeclNode;
struct StructDeclNode;
struct EnumNode;
struct FunctionDeclNode;
struct VariableDeclListNode;
struct IfNode;
struct WhileNode;
struct DoWhileNode;
struct ForNode;
struct CaseNode;
struct SwitchNode;
struct AST;

class Parser
{
private:
    std::vector<Token> m_Tokens;
    size_t m_Current;

    bool match(const TokenType& type);
    Token prev();
    void goBack(unsigned n);

    Node* parseTopStatement();
    Node* parseStatement();

    ClassDeclNode* parseClass();
    StructDeclNode* parseStruct();
    EnumNode* parseEnum();
    FunctionDeclNode* parseFunction();
    VariableDeclListNode* parseVariableDeclList();
    ExpressionNode* parseExpression();
    IfNode* parseIf();
    WhileNode* parseWhile();
    DoWhileNode* parseDoWhile();
    ForNode* parseFor();
    SwitchNode* parseSwitch();

    ExpressionNode* parseIdentifier();
    ExpressionNode* parseLiteral();
    ExpressionNode* parseScopeRes();
    ExpressionNode* parseUnaryPostfix();
    ExpressionNode* parseUnaryPrefix();
    ExpressionNode* parsePtrToMember();
    ExpressionNode* parseProd();
    ExpressionNode* parseSum();
    ExpressionNode* parseBitShift();
    ExpressionNode* parseRelational();
    ExpressionNode* parseEquality();
    ExpressionNode* parseBitAnd();
    ExpressionNode* parseBitXor();
    ExpressionNode* parseBitOr();
    ExpressionNode* parseAnd();
    ExpressionNode* parseOr();
    ExpressionNode* parseAssignment();
    ExpressionNode* parseComma();
    ExpressionNode* parseExpressionStatement();

public:
    Parser(const std::vector<Token>& tokens);
    AST parse();
};

#endif // PARSER_H
