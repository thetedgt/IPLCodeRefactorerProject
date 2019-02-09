#ifndef AST_H
#define AST_H

#include "Parser.h"

struct ASTPrinter;
struct CodeRefactorer;

enum class DeclarationType : unsigned char
{
    BOOL,
    CHAR,
    DOUBLE,
    FLOAT,
    INT,
    VOID,
    WCHAR_T
};

// enum class ExpressionType : unsigned char
// {
//     // Unary
//     PLUS_PLUS,
//     MINUS_MINUS,
//     NOT,

//     // Binary
//     ADD,
//     SUB,
//     MULT,
//     DIV,
//     MOD,
//     ASSIGN,
//     IS_EQUAL,
//     IS_LESSER,
//     IS_GREATER,
//     IS_NOT_EQUAL,
//     IS_LESS_EQUAL,
//     IS_GREAT_EQUAL,
    
//     // Other
//     IDENTIFIER,

// };

struct Node
{
    virtual ~Node() {};
    virtual void print(ASTPrinter& printer) = 0;
    virtual void refactor(CodeRefactorer& ref) = 0;
};

struct ExpressionNode : Node
{
    Token type;
    //FunctionCallNode *funcCall;
    ExpressionNode *lhs;
    ExpressionNode *rhs;

    ExpressionNode(const Token& _type, ExpressionNode* _lhs, ExpressionNode* _rhs) :
        type(_type), lhs(_lhs), rhs(_rhs)
    {}

    ExpressionNode(const ExpressionNode&) = default;//delete;
    ExpressionNode& operator=(const ExpressionNode&) = delete;

    virtual ~ExpressionNode() override
    {
        delete lhs;
        delete rhs;
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct TypeNode : Node
{
    DeclarationType kind;

    TypeNode(const DeclarationType& _kind) :
        kind(_kind)
    {}

    TypeNode(const TypeNode&) = default;//delete;
    TypeNode& operator=(const TypeNode&) = delete;

    virtual ~TypeNode() = default;

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};


struct ClassDeclNode : Node
{
    Token id;
    std::list<std::pair<TokenType, Token>> inherits;
    std::list<Node*> privates;
    std::list<Node*> protecteds;
    std::list<Node*> publics;

    ClassDeclNode(const Token& _id, const std::list<std::pair<TokenType, Token>>& _inherits, const std::list<Node*>& _privates,
                    const std::list<Node*>& _protecteds, const std::list<Node*>& _publics) :
        id(_id), inherits(_inherits), privates(_privates), protecteds(_protecteds), publics(_publics)
    {}

    ClassDeclNode(const ClassDeclNode&) = default;// delete;
    ClassDeclNode& operator=(const ClassDeclNode&) = delete;

    virtual ~ClassDeclNode()
    {
        for(auto i : privates)
        {
            delete i;
        }
        for(auto i : protecteds)
        {
            delete i;
        }
        for(auto i : publics)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
}; 

struct StructDeclNode : Node
{
    Token id;
    std::list<std::pair<TokenType, Token>> inherits;
    std::list<Node*> privates;
    std::list<Node*> protecteds;
    std::list<Node*> publics;

    StructDeclNode(const Token& _id, const std::list<std::pair<TokenType, Token>>& _inherits, const std::list<Node*>& _privates,
                    const std::list<Node*>& _protecteds, const std::list<Node*>& _publics) :
        id(_id), inherits(_inherits), privates(_privates), protecteds(_protecteds), publics(_publics)
    {}

    StructDeclNode(const StructDeclNode&) = default;// delete;
    StructDeclNode& operator=(const StructDeclNode&) = delete;

    virtual ~StructDeclNode()
    {
        for(auto i : privates)
        {
            delete i;
        }
        for(auto i : protecteds)
        {
            delete i;
        }
        for(auto i : publics)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
}; 

struct EnumNode : Node
{
    Token id;
    bool isEnumClass;
    TypeNode type;
    std::list<std::pair<Token, ExpressionNode*>> vals;

    EnumNode(const Token& _id, bool _isClass, const TypeNode& _type, const std::list<std::pair<Token, ExpressionNode*>>& _vals) :
        id(_id), isEnumClass(_isClass), type(_type), vals(_vals)
    {}

    EnumNode(const EnumNode&) = default; //delete;
    EnumNode& operator=(const EnumNode&) = delete;

    virtual ~EnumNode()
    {
        for(auto pair : vals)
        {
            delete pair.second;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct FunctionDeclNode : Node
{
    TypeNode returnType;
    Token id;
    std::list<std::pair<TypeNode, Token>> params;
    std::list<Node*> body;

    FunctionDeclNode(const TypeNode& rt, const Token& name, 
        const std::list<std::pair<TypeNode, Token>>& params_, const std::list<Node*>& body_) :
        returnType(rt), id(name), params(params_), body(body_)
    {}

    FunctionDeclNode(const FunctionDeclNode&) = default;// delete;
    FunctionDeclNode& operator=(const FunctionDeclNode&) = delete;

    virtual ~FunctionDeclNode()
    {
        for(auto i : body)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct VariableDeclListNode : Node
{
    TypeNode type;
    std::list<std::pair<Token, ExpressionNode*>> decls;

    VariableDeclListNode(const TypeNode& _type, const std::list<std::pair<Token, ExpressionNode*>>& _decls) :
        type(_type), decls(_decls)
    {}

    VariableDeclListNode(const VariableDeclListNode&) = default;// delete;
    VariableDeclListNode& operator=(const VariableDeclListNode&) = delete;

    virtual ~VariableDeclListNode()
    {
        for(auto pair : decls)
        {
            delete pair.second;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct IfNode : Node
{
    ExpressionNode *condition;
    std::list<Node*> thenBody;
    std::list<Node*> elseBody;

    IfNode(ExpressionNode* _cond, const std::list<Node*>& _then, const std::list<Node*>& _else) :
        condition(_cond), thenBody(_then), elseBody(_else)
    {}

    IfNode(const IfNode&) = default;// delete;
    IfNode& operator=(const IfNode&) = delete;

    virtual ~IfNode()
    {
        delete condition;
        for(auto i : thenBody)
        {
            delete i;
        }
        for(auto i : elseBody)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct WhileNode : Node
{
    ExpressionNode *condition;
    std::list<Node*> body;

    WhileNode(ExpressionNode* _cond, const std::list<Node*> _body) :
        condition(_cond), body(_body)
    {}

    WhileNode(const WhileNode&) = default; //delete;
    WhileNode& operator=(const WhileNode&) = delete;

    virtual ~WhileNode()
    {
        delete condition;
        for(auto i : body)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct DoWhileNode : Node
{
    ExpressionNode *condition;
    std::list<Node*> body;

    DoWhileNode(ExpressionNode* _cond, const std::list<Node*> _body) :
        condition(_cond), body(_body)
    {}

    DoWhileNode(const DoWhileNode&) = default; //delete;
    DoWhileNode& operator=(const DoWhileNode&) = delete;

    virtual ~DoWhileNode()
    {
        delete condition;
        for(auto i : body)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct ForNode : Node
{
    ExpressionNode *init, *cond, *iter;
    std::list<Node*> body;

    ForNode(ExpressionNode* _init, ExpressionNode* _cond, ExpressionNode* _iter, const std::list<Node*>& _body) :
        init(_init), cond(_cond), iter(_iter), body(_body)
    {}

    ForNode(const ForNode&) = default;// delete;
    ForNode& operator=(const ForNode&) = delete;

    virtual ~ForNode()
    {
        delete init;
        delete cond;
        delete iter;
        for(auto i : body)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct CaseNode : Node
{
    Token caseVal; // must be literal or const expr
    std::list<Node*> body;

    CaseNode(const Token& _caseVal, const std::list<Node*>& _body) :
        caseVal(_caseVal), body(_body)
    {}

    CaseNode(const CaseNode&) = default; //delete;
    CaseNode& operator=(const CaseNode&) = delete;

    virtual ~CaseNode()
    {
        for(auto i : body)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct SwitchNode : Node
{
    ExpressionNode* cond;
    std::list<CaseNode*> cases;

    SwitchNode(ExpressionNode* _cond, const std::list<CaseNode*>& _cases) :
        cond(_cond), cases(_cases)
    {}

    SwitchNode(const SwitchNode&) = default;// delete;
    SwitchNode& operator=(const SwitchNode&) = delete;

    virtual ~SwitchNode()
    {
        delete cond;
        for(auto i : cases)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

struct AST : Node
{
    std::list<Node*> topStatements;

    AST() {}
    AST(const std::list<Node*>& ts) :
        topStatements(ts)
    {}

    AST(const AST&) = default;// delete;
    AST& operator=(const AST&) = delete;

    virtual ~AST()
    {
        for(auto i : topStatements)
        {
            delete i;
        }
    }

    virtual void print(ASTPrinter& printer) override;
    virtual void refactor(CodeRefactorer& ref) override;
};

#endif