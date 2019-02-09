#include "CodeRefactorer.h"
#include <iostream>


void CodeRefactorer::visit(ExpressionNode& node)
{
    if(node.type.type == TokenType::IDENTIFIER && node.type.lexeme == toFind)
    {
        st.front().useLines.push_back(node.type.line);
    }
    else
    {
        if(node.lhs) node.lhs->refactor(*this);
        if(node.rhs) node.rhs->refactor(*this);
    }
}

void CodeRefactorer::visit(TypeNode& node)
{
    // needs to be reworked if parsing more complex types;
    return;
}

void CodeRefactorer::visit(ClassDeclNode& node)
{
    //std::cout << "in class\n";
    if(node.id.lexeme == toFind)
    {
        state = true;
        Occurance oc(Occurance::IDType::CLASS, node.id.line);
        st.push_front(oc);

        // if a member uses the type it has to be a ptr to it
        // if a method uses the type it can be the type, ptr to it, or ref to it
    }
    for(auto in : node.inherits)
    {
        Token tkn = in.second;
        if(tkn.lexeme == toFind)
        {
            st.front().useLines.push_back(tkn.line);
        }
    }

    //std::cout << "  in class body\n";
    bool prevState = state;
    state = false;
    for(auto i : node.privates)
    {
        i->refactor(*this);
    }
    for(auto i : node.protecteds)
    {
        i->refactor(*this);
    }
    for(auto i : node.publics)
    {
        i->refactor(*this);
    }

    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = prevState;
}

void CodeRefactorer::visit(StructDeclNode& node)
{
    //std::cout << "in struct\n";
    if(node.id.lexeme == toFind)
    {
        state = true;
        Occurance oc(Occurance::IDType::CLASS, node.id.line);
        st.push_front(oc);

        // if a member uses the type it has to be a ptr to it
        // if a method uses the type it can be the type, ptr to it, or ref to it
    }
    for(auto in : node.inherits)
    {
        Token tkn = in.second;
        if(tkn.lexeme == toFind)
        {
            st.front().useLines.push_back(tkn.line);
        }
    }

    //std::cout << "  in struct body\n";
    bool prevState = state;
    state = false;
    for(auto i : node.privates)
    {
        i->refactor(*this);
    }
    for(auto i : node.protecteds)
    {
        i->refactor(*this);
    }
    for(auto i : node.publics)
    {
        i->refactor(*this);
    }

    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = prevState;
}

void CodeRefactorer::visit(EnumNode& node)
{
    //std::cout << "in enum\n";
    // TO DO need to give it more thought 

    if(node.id.lexeme == toFind)
    {
        state = true;
        Occurance oc(Occurance::IDType::ENUM, node.id.line);
        st.push_front(oc);
    }

    //std::cout << "  in enum body\n";
    for(auto pair : node.vals)
    {
        if(pair.first.lexeme == toFind)
        {
            state = true;
            Occurance oc(Occurance::IDType::ENUMERATOR, node.id.line);
            st.push_front(oc);
        }
    }   
}

void CodeRefactorer::visit(FunctionDeclNode& node)
{
    //std::cout << "in func decl\n";
    // check return type after implementing more complex types;
    if(node.id.lexeme == toFind)
    {
        state = true;
        Occurance oc(Occurance::IDType::FUNCTION, node.id.line);
        st.push_front(oc);
    }
    bool idState = state;
    state = false;

    //std::cout << "  in func param list\n";
    for(auto param : node.params)
    {
        if(param.second.lexeme == toFind)
        {
            state = true;
            Occurance oc(Occurance::IDType::PARAM, node.id.line);
            st.push_front(oc);
        }
    }

    //std::cout << "  in func body\n";
    for(auto i : node.body)
    {
        i->refactor(*this);
    }
    //std::cout << "  out of func body\n";
    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = idState;
}

void CodeRefactorer::visit(VariableDeclListNode& node)
{
    //std::cout << "in var decl list\n";
    for(auto var : node.decls)
    {
        //std::cout << var.first.lexeme << std::endl;
        if(var.first.lexeme == toFind)
        {
            //std::cout << "found\n";
            state = true;
            Occurance oc(Occurance::IDType::VARIABLE, var.first.line);
            st.push_front(oc);
        }
        if(var.second)
        {
            var.second->refactor(*this);
        }
    }
}

void CodeRefactorer::visit(IfNode& node)
{
    //std::cout << "in if\n";
    bool prevState = state;
    state = false;
    // check if there are no variable decls in the cond
    node.condition->refactor(*this);

    //std::cout << "  in then body\n";
    for(auto i : node.thenBody)
    {
        i->refactor(*this);
    }
    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
        state = false;
    }
    //std::cout << "  in else body\n";
    for(auto i : node.elseBody)
    {
        i->refactor(*this);
    }

    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = prevState;
}

void CodeRefactorer::visit(WhileNode& node)
{
    //std::cout << "in while\n";
    bool prevState = state;
    state = false;
    // check if there are no variable decls in the cond
    node.condition->refactor(*this);

    //std::cout << "in while body\n";
    for(auto i : node.body)
    {
        i->refactor(*this);
    }
    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = prevState;
}

void CodeRefactorer::visit(DoWhileNode& node)
{
    //std::cout << "in do while\n";
    bool prevState = state;
    state = false;

    //std::cout << "  in do while body\n";
    for(auto i : node.body)
    {
        i->refactor(*this);
    }
    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = prevState;
    node.condition->refactor(*this);
}

void CodeRefactorer::visit(ForNode& node)
{
    //std::cout << "in for\n";
    bool prevState = state;
    state = false;

    if(node.init) node.init->refactor(*this);
    if(node.cond) node.cond->refactor(*this);
    if(node.iter) node.iter->refactor(*this);

    //std::cout << "  in for body\n";
    for(auto i : node.body)
    {
        i->refactor(*this);
    }

    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = prevState;
}

void CodeRefactorer::visit(CaseNode& node)
{
    //std::cout << "In case\n";
    bool prevState = state;
    state = false;
    if(node.caseVal.type == TokenType::IDENTIFIER && node.caseVal.lexeme == toFind)
    {
        st.front().useLines.push_back(node.caseVal.line);
    }

    for(auto i : node.body)
    {
        i->refactor(*this);
    }
    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = prevState;
}

void CodeRefactorer::visit(SwitchNode& node)
{
    //std::cout << "In switch\n";
    bool prevState = state;
    state = false;
    // check if there are no variable decls in the cond
    node.cond->refactor(*this);

    for(auto i : node.cases)
    {
        i->refactor(*this);
    }
    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
    state = prevState;
}

void CodeRefactorer::visit(AST& node)
{
    state = false;
    for(auto i : node.topStatements)
    {
        //std::cout << "Entering top statement\n";
        i->refactor(*this);
    }
    if(state)
    {
        occurances.push_back(st.front());
        st.pop_front();
    }
}

void CodeRefactorer::replace(const std::string& replacement, std::vector<Token>& tokens)
{
    if(occurances.empty())
    {
        std::cout << "There is nothing to replace.\n";
        return;
    }

    int index, len = occurances.size();
    do
    {
        std::cout << "Which occurance of " << toFind << " would you like to replace?\n> ";
        std::cin >> index;
    } while(index < 1 || index > len);

    auto oc = occurances.begin();
    while(index > 1)
    {
        --index;
        ++oc;
    }

    auto tkn = tokens.begin();
    for(; tkn != tokens.end(); ++tkn)
    {
        if((*tkn).type == TokenType::IDENTIFIER && (*tkn).line == (*oc).defLine && (*tkn).lexeme == toFind)
        {
            (*tkn).lexeme = replacement;
            break;
        }
    }

    for(auto use : (*oc).useLines)
    {
        while((*tkn).type != TokenType::IDENTIFIER || (*tkn).line != use || (*tkn).lexeme != toFind)
        {
            ++tkn;
        }
        (*tkn).lexeme = replacement;
    }

}