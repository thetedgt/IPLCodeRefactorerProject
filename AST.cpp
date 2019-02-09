#include "AST.h"
#include "ASTPrinter.h"
#include "CodeRefactorer.h"

void ExpressionNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void TypeNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void ClassDeclNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void StructDeclNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void EnumNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void FunctionDeclNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void VariableDeclListNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void IfNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void WhileNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void DoWhileNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void ForNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void CaseNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void SwitchNode::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void AST::print(ASTPrinter& printer)
{
    printer.visit(*this);
}

void ExpressionNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void TypeNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void ClassDeclNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void StructDeclNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void EnumNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void FunctionDeclNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void VariableDeclListNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void IfNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void WhileNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void DoWhileNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void ForNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void CaseNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void SwitchNode::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}

void AST::refactor(CodeRefactorer& ref)
{
    ref.visit(*this);
}