#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H

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


struct NodeVisitor
{
    virtual ~NodeVisitor() {};
    virtual void visit(ExpressionNode& node) = 0;
    virtual void visit(TypeNode& node) = 0;
    virtual void visit(ClassDeclNode& node) = 0;
    virtual void visit(StructDeclNode& node) = 0;
    virtual void visit(EnumNode& node) = 0;
    virtual void visit(FunctionDeclNode& node) = 0;
    virtual void visit(VariableDeclListNode& node) = 0;
    virtual void visit(IfNode& node) = 0;
    virtual void visit(WhileNode& node) = 0;
    virtual void visit(DoWhileNode& node) = 0;
    virtual void visit(ForNode& node) = 0;
    virtual void visit(CaseNode& node) = 0;
    virtual void visit(SwitchNode& node) = 0;
    virtual void visit(AST& node) = 0;
};

#endif