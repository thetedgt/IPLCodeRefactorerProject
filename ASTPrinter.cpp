#include "ASTPrinter.h"

void ASTPrinter::visit(ExpressionNode& node)
{
    if(node.lhs) node.lhs->print(*this);

    if(node.type.type == TokenType::NUMBER) std::cout << node.type.value << " ";
    else std::cout << node.type.lexeme << " ";

    if(node.rhs) node.rhs->print(*this);
}

void ASTPrinter::visit(TypeNode& node)
{
    switch(node.kind)
    {
        case DeclarationType::BOOL: std::cout << "bool ";break;
        case DeclarationType::CHAR: std::cout << "char ";break;
        case DeclarationType::DOUBLE: std::cout << "double ";break;
        case DeclarationType::FLOAT: std::cout << "float ";break;
        case DeclarationType::INT: std::cout << "int ";break;
        case DeclarationType::VOID: std::cout << "void ";break;
        case DeclarationType::WCHAR_T: std::cout << "wchar_t ";break;
    }
}

void ASTPrinter::visit(ClassDeclNode& node)
{
    std::cout << "class " << node.id.lexeme << " ";
    if(!node.inherits.empty())
    {
        std::cout << ": ";
        for(auto parent : node.inherits)
        {
            switch(parent.first)
            {
                case TokenType::PRIVATE: std::cout << "private ";
                case TokenType::PROTECTED: std::cout << "protected ";
                case TokenType::PUBLIC: std::cout << "public ";
            }
            std::cout << parent.second.lexeme << ", ";
        }
    }

    std::cout << "\n{\n";

    if(!node.privates.empty())
    {
        std::cout << "private:\n";
        for(auto priv : node.privates)
        {
            priv->print(*this);
        }
        std::cout << std::endl;
    }

    if(!node.protecteds.empty())
    {
        std::cout << "protected:\n";
        for(auto prot : node.protecteds)
        {
            prot->print(*this);
        }
        std::cout << std::endl;
    }

    if(!node.publics.empty())
    {
        std::cout << "public:\n";
        for(auto pub : node.publics)
        {
            pub->print(*this);
        }
        std::cout << std::endl;
    }

    std::cout << "}\n\n";
}

void ASTPrinter::visit(StructDeclNode& node)
{
    std::cout << "struct " << node.id.lexeme << " ";
    if(!node.inherits.empty())
    {
        std::cout << ": ";
        for(auto parent : node.inherits)
        {
            switch(parent.first)
            {
                case TokenType::PRIVATE: std::cout << "private ";
                case TokenType::PROTECTED: std::cout << "protected ";
                case TokenType::PUBLIC: std::cout << "public ";
            }
            std::cout << parent.second.lexeme << ", ";
        }
    }

    std::cout << "\n{\n";

    if(!node.privates.empty())
    {
        std::cout << "private:\n";
        for(auto priv : node.privates)
        {
            priv->print(*this);
        }
        std::cout << std::endl;
    }

    if(!node.protecteds.empty())
    {
        std::cout << "protected:\n";
        for(auto prot : node.protecteds)
        {
            prot->print(*this);
        }
        std::cout << std::endl;
    }

    if(!node.publics.empty())
    {
        std::cout << "public:\n";
        for(auto pub : node.publics)
        {
            pub->print(*this);
        }
        std::cout << std::endl;
    }

    std::cout << "}\n\n";
}

void ASTPrinter::visit(EnumNode& node)
{
    std::cout << "enum ";
    if(node.isEnumClass) std::cout << "class ";
    std::cout << node.id.lexeme << " : ";
    node.type.print(*this);

    std::cout << "\n{\n";
    for(auto val : node.vals)
    {
        std::cout << val.first.lexeme << " ";
        if(val.second)
        {
            std::cout << "= ";
            val.second->print(*this);
        }
        std::cout << ",\n";
    }
    std::cout << "}\n\n";
}

void ASTPrinter::visit(FunctionDeclNode& node)
{
    node.returnType.print(*this);
    std::cout << node.id.lexeme << "(";
    for(auto param : node.params)
    {
        param.first.print(*this);
        std::cout << " " << param.second.lexeme << ", "; 
    }
    std::cout << ")\n{\n";

    for(auto stmt : node.body)
    {
        stmt->print(*this);
    }
    std::cout << "}\n\n";
}

void ASTPrinter::visit(VariableDeclListNode& node)
{
    node.type.print(*this);
    std::cout << " ";
    for(auto var : node.decls)
    {
        std::cout << var.first.lexeme;
        if(var.second)
        {
            std::cout << " = ";
            var.second->print(*this);
        }
        std::cout << ", "; 
    }
    std::cout << ";\n";

}

void ASTPrinter::visit(IfNode& node)
{
    std::cout << "if(";
    node.condition->print(*this);
    std::cout << ")\n{\n";
    for(auto then : node.thenBody)
    {
        then->print(*this);
        std::cout << "\n";
    }
    std::cout << "}\n";
    if(!node.elseBody.empty())
    {
        std::cout << "else\n{\n";
        for(auto e : node.elseBody)
        {
            e->print(*this);
            std::cout << std::endl;
        }
        std::cout << "}\n";
    }
}

void ASTPrinter::visit(WhileNode& node)
{
    std::cout << "while(";
    node.condition->print(*this);
    std::cout << ")\n{\n";
    for(auto stmt : node.body)
    {
        stmt->print(*this);
        std::cout << "\n";
    }
    std::cout << "}\n";
}

void ASTPrinter::visit(DoWhileNode& node)
{
    std::cout << "do {\n";
    for(auto stmt : node.body)
    {
        stmt->print(*this);
        std::cout << "\n";
    }
    std::cout << "} while(";
    node.condition->print(*this);
    std::cout << ");\n";
}

void ASTPrinter::visit(ForNode& node)
{
    std::cout << "for(";
    if(node.init) node.init->print(*this);
    std::cout << "; ";
    if(node.cond) node.cond->print(*this);
    std::cout << "; ";
    if(node.iter) node.iter->print(*this);
    std::cout << ")\n{";
    for(auto stmt : node.body)
    {
        stmt->print(*this);
        std::cout << "\n";
    }
    std::cout << "}\n";
}

void ASTPrinter::visit(CaseNode& node)
{
    std::cout << "case ";
    if(node.caseVal.type == TokenType::NUMBER) std::cout << node.caseVal.value;
    else std::cout << node.caseVal.lexeme;
    std::cout << ": ";
    for(auto stmt : node.body)
    {
        stmt->print(*this);
        std::cout << "\n";
    }
}

void ASTPrinter::visit(SwitchNode& node)
{
    std::cout << "switch(";
    node.cond->print(*this);
    std::cout << ")\n{";
    for(auto stmt : node.cases)
    {
        stmt->print(*this);
    }
    std::cout << "}\n"; 
}

void ASTPrinter::visit(AST& node)
{
    for(auto stmt : node.topStatements)
    {
        stmt->print(*this);
    }
}