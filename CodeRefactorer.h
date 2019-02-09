#ifndef CODE_REFACTORER_H
#define CODE_REFACTORER_H

#include "NodeVisitor.h"
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

struct CodeRefactorer : NodeVisitor
{
    std::string toFind;

    struct Occurance
    {
        enum IDType
        {
            CLASS,
            ENUM,
            ENUMERATOR,
            FUNCTION,
            PARAM,
            VARIABLE
        } type;
        unsigned defLine;
        std::list<unsigned> useLines;

        Occurance(IDType t, unsigned line) :
            type(t), defLine(line)
        {}
    };
    std::list<Occurance> occurances, st;
    bool state; // 0 - not found, 1 - def found

    CodeRefactorer(const std::string& str) :
        toFind(str), state(false)
    {}

    virtual ~CodeRefactorer() {};

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

    void replace(const std::string& replacement, std::vector<Token>& tokens);
};

#endif