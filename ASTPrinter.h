#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "NodeVisitor.h"
#include "AST.h"
#include <iostream>

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

struct ASTPrinter : NodeVisitor
{
    virtual ~ASTPrinter() {};
    virtual void visit(ExpressionNode& node) override;
    virtual void visit(TypeNode& node) override;
    virtual void visit(ClassDeclNode& node) override;
    virtual void visit(StructDeclNode& node) override;
    virtual void visit(EnumNode& node) override;
    virtual void visit(FunctionDeclNode& node) override;
    virtual void visit(VariableDeclListNode& node) override;
    virtual void visit(IfNode& node) override;
    virtual void visit(WhileNode& node) override;
    virtual void visit(DoWhileNode& node) override;
    virtual void visit(ForNode& node) override;
    virtual void visit(CaseNode& node) override;
    virtual void visit(SwitchNode& node) override;
    virtual void visit(AST& node) override;
};

#endif