#include "Parser.h"

bool Parser::match(const TokenType& t)
{
    if(m_Tokens[m_Current].type == t)
    {
        ++m_Current;
        return true;
    }
    return false;
}

ClassDeclNode* Parser::parseClass()
{
    if(match(TokenType::CLASS))
    {
        Token id;
        std::list<std::pair<TokenType, Token>> inherits;
        std::list<Node*> privates;
        std::list<Node*> protecteds;
        std::list<Node*> publics;
        bool isAnnonymus;
        if(match(TokenType::IDENTIFIER))
        {
            id = prev();
            isAnnonymus = false;
        }
        else // annonymus class must have defined an object
        {
            isAnnonymus = true;
        }

        if(match(TokenType::COLON))
        {
            TokenType type;
            Token classID;
            do
            {   
                if(match(TokenType::PRIVATE) || match(TokenType::PROTECTED) || match(TokenType::PUBLIC))
                {
                    type = prev().type;
                }
                else 
                {
                    type = TokenType::PRIVATE;
                }

                if(match(TokenType::IDENTIFIER))
                {
                    classID = prev();
                }
                else
                {
                    exit(1);
                }

                inherits.push_back(std::make_pair(type, classID));
            } while(match(TokenType::COMMA));
        }

        if(match(TokenType::LEFT_CUR_BRAC))
        {
            while(!match(TokenType::RIGHT_CUR_BRAC))
            {
                if(match(TokenType::PRIVATE))
                {
                    if(match(TokenType::COLON))
                    {
                        Node* temp;
                        while(temp = parseTopStatement())
                        {
                            privates.push_back(temp);
                        }
                    }
                    else
                    {
                        exit(1);
                    }
                }
                else
                {
                    Node* temp;
                    while(temp = parseTopStatement())
                    {
                        privates.push_back(temp);
                    }
                }
            }
        }

        // TODO: need to match variable decl before ;

        if(match(TokenType::SEMI_COLON))
        {
            ClassDeclNode *node = new ClassDeclNode(id, inherits, privates, protecteds, publics);
            return node;
        }
    }
    return nullptr;
}

StructDeclNode* Parser::parseStruct()
{
    if(match(TokenType::STRUCT))
    {
        Token id;
        std::list<std::pair<TokenType, Token>> inherits;
        std::list<Node*> privates;
        std::list<Node*> protecteds;
        std::list<Node*> publics;
        bool isAnnonymus;
        if(match(TokenType::IDENTIFIER))
        {
            id = prev();
            isAnnonymus = false;
        }
        else // annonymus class must have defined an object
        {
            isAnnonymus = true;
        }

        if(match(TokenType::COLON))
        {
            TokenType type;
            Token classID;
            do
            {   
                if(match(TokenType::PRIVATE) || match(TokenType::PROTECTED) || match(TokenType::PUBLIC))
                {
                    type = prev().type;
                }
                else 
                {
                    type = TokenType::PUBLIC;
                }

                if(match(TokenType::IDENTIFIER))
                {
                    classID = prev();
                }
                else
                {
                    exit(1);
                }

                inherits.push_back(std::make_pair(type, classID));
            } while(match(TokenType::COMMA));
        }

        if(match(TokenType::LEFT_CUR_BRAC))
        {
            while(!match(TokenType::RIGHT_CUR_BRAC))
            {
                if(match(TokenType::PRIVATE))
                {
                    if(match(TokenType::COLON))
                    {
                        Node* temp;
                        while(temp = parseTopStatement())
                        {
                            privates.push_back(temp);
                        }
                    }
                    else
                    {
                        exit(1);
                    }
                }
                else
                {
                    Node* temp;
                    while(temp = parseTopStatement())
                    {
                        privates.push_back(temp);
                    }
                }
            }
        }

        // TODO: need to match variable decl before ;

        if(match(TokenType::SEMI_COLON))
        {
            StructDeclNode *node = new StructDeclNode(id, inherits, privates, protecteds, publics);
            return node;
        }
    }
    return nullptr;
}

FunctionDeclNode* Parser::parseFunction()
{
    // TODO: user-defined types,constructors (with init lists), destructors, operators,
    // better type parsing (qualifiers, pointers, references, etc), parsing ::, auto,
    // higher order functions
    unsigned into = 0;
    TypeNode type(DeclarationType::BOOL);
    if(match(TokenType::BOOL)) type.kind = DeclarationType::BOOL;
    else if (match(TokenType::CHAR)) type.kind = DeclarationType::CHAR;
    else if (match(TokenType::DOUBLE)) type.kind = DeclarationType::DOUBLE;
    else if (match(TokenType::FLOAT)) type.kind = DeclarationType::FLOAT;
    else if (match(TokenType::INT)) type.kind = DeclarationType::INT;
    else if (match(TokenType::VOID)) type.kind = DeclarationType::VOID;
    else if (match(TokenType::WCHAR_T)) type.kind = DeclarationType::WCHAR_T;
    else return nullptr;

    ++into;

    Token id;
    if(match(TokenType::IDENTIFIER))
    {
        id = prev();
    }
    else 
    {
        goBack(into);
        return nullptr;
    }

    ++into;

    std::list<std::pair<TypeNode, Token>> params;
    if(match(TokenType::LEFT_PAREN))
    {
        if(!match(TokenType::RIGHT_PAREN))
        {
            do
            {
                TypeNode paramType(DeclarationType::BOOL);
                Token param;
                if(match(TokenType::BOOL)) paramType.kind = DeclarationType::BOOL;
                else if (match(TokenType::CHAR)) paramType.kind = DeclarationType::CHAR;
                else if (match(TokenType::DOUBLE)) paramType.kind = DeclarationType::DOUBLE;
                else if (match(TokenType::FLOAT)) paramType.kind = DeclarationType::FLOAT;
                else if (match(TokenType::INT)) paramType.kind = DeclarationType::INT;
                //else if (match(TokenType::VOID)) paramType.kind = DeclarationType::VOID;
                else if (match(TokenType::WCHAR_T)) paramType.kind = DeclarationType::WCHAR_T;
                else return nullptr;

                if(match(TokenType::IDENTIFIER))
                {
                    param = prev();
                }

                params.push_back(std::make_pair(paramType, param));
            }
            while(match(TokenType::COMMA));
            if(!match(TokenType::RIGHT_PAREN))
            {
                return nullptr;
            }
        }
    }
    else 
    {   
        goBack(into);
        return nullptr;
    }

    std::list<Node*> body;
    if(match(TokenType::LEFT_CUR_BRAC))
    {
        Node* statement;
        while(statement = parseStatement())
        {
            // check if it is a break statement
            body.push_back(statement);
        }

        if(!match(TokenType::RIGHT_CUR_BRAC))
        {
            return nullptr;
        }
    }
    else if(!match(TokenType::SEMI_COLON))
    {
        return nullptr;
    }

    FunctionDeclNode* temp = new FunctionDeclNode(type, id, params, body);
    return temp;
}

VariableDeclListNode* Parser::parseVariableDeclList()
{
    // TO DO: parse more types
    TypeNode type(DeclarationType::BOOL);
    if(match(TokenType::BOOL)) type.kind = DeclarationType::BOOL;
    else if (match(TokenType::CHAR)) type.kind = DeclarationType::CHAR;
    else if (match(TokenType::DOUBLE)) type.kind = DeclarationType::DOUBLE;
    else if (match(TokenType::FLOAT)) type.kind = DeclarationType::FLOAT;
    else if (match(TokenType::INT)) type.kind = DeclarationType::INT;
    //else if (match(TokenType::VOID)) type.kind = DeclarationType::VOID;
    else if (match(TokenType::WCHAR_T)) type.kind = DeclarationType::WCHAR_T;
    else return nullptr;

    //std::cout << "parsed variable decl type\n";

    std::list<std::pair<Token, ExpressionNode*>> decls;
    do
    {
        Token id;
        ExpressionNode* exp = nullptr;
        if(match(TokenType::IDENTIFIER))
        {
            id = prev();
        }
        else return nullptr;

        if(match(TokenType::ASSIGN))
        {
            exp = parseExpression();
        }

        //std::cout << id.lexeme << std::endl;

        decls.push_back(std::make_pair(id, exp));
    }
    while(match(TokenType::COMMA));

    if(match(TokenType::SEMI_COLON))
    {
        VariableDeclListNode* temp = new VariableDeclListNode(type, decls);
        return temp;
    }
    return nullptr;
}

EnumNode* Parser::parseEnum()
{
    if(match(TokenType::ENUM))
    {
        bool isEnumClass = false;
        if(match(TokenType::CLASS) || match(TokenType::STRUCT))
        {
            isEnumClass = true;
        }

        Token id;
        if(match(TokenType::IDENTIFIER))
        {
            id = prev();
        }

        TypeNode type(DeclarationType::INT);
        if(match(TokenType::COLON))
        {
            if(match(TokenType::BOOL)) type.kind = DeclarationType::BOOL;
            else if (match(TokenType::CHAR)) type.kind = DeclarationType::CHAR;
            else if (match(TokenType::INT)) type.kind = DeclarationType::INT;
            else if (match(TokenType::WCHAR_T)) type.kind = DeclarationType::WCHAR_T;
            else return nullptr;
        }

        std::list<std::pair<Token, ExpressionNode*>> vals;
        if(match(TokenType::LEFT_CUR_BRAC))
        {
            while(!match(TokenType::RIGHT_CUR_BRAC))
            {
                Token val;
                ExpressionNode* exp;
                if(match(TokenType::IDENTIFIER))
                {
                    val = prev();
                }

                if(match(TokenType::ASSIGN))
                {
                    // TO DO: check if expression is const
                    exp = parseExpression();
                }

                match(TokenType::COMMA);
            }
        }

        // TO DO: parse variable decl before ;

        if(match(TokenType::SEMI_COLON))
        {
            EnumNode* node = new EnumNode(id, isEnumClass, type, vals);
            return node;
        }        
    }
    return nullptr;
}

IfNode* Parser::parseIf()
{
    if(match(TokenType::IF))
    {
        ExpressionNode* cond;
        if(match(TokenType::LEFT_PAREN))
        {
            cond = parseExpression();
            if(!cond || !match(TokenType::RIGHT_PAREN))
            {
                return nullptr;
            }
        }

        std::list<Node*> thenBody;
        if(match(TokenType::LEFT_CUR_BRAC))
        {
            Node* stmt;
            while(!match(TokenType::RIGHT_CUR_BRAC))
            {
                stmt = parseStatement();
                if(!stmt) exit(1);
                thenBody.push_back(stmt);
            }
        }
        else if(!match(TokenType::SEMI_COLON))
        {
            Node* stmt = parseStatement();
            if(!stmt) exit(1);
            thenBody.push_back(stmt);
        }

        std::list<Node*> elseBody;
        if(match(TokenType::ELSE))
        {
            if(match(TokenType::LEFT_CUR_BRAC))
            {
                Node* stmt;
                while(!match(TokenType::RIGHT_CUR_BRAC))
                {
                    stmt = parseStatement();
                    if(!stmt) exit(1);
                    elseBody.push_back(stmt);
                }
            }
            else if(!match(TokenType::SEMI_COLON))
            {
                Node* stmt = parseStatement();
                if(!stmt) exit(1);
                elseBody.push_back(stmt);
            }
        }

        IfNode* temp = new IfNode(cond, thenBody, elseBody);
        return temp;
    }
    return nullptr;
}

WhileNode* Parser::parseWhile()
{
    if(match(TokenType::WHILE))
    {
        ExpressionNode* cond;
        if(match(TokenType::LEFT_PAREN))
        {
            cond = parseExpression();
            if(!cond || !match(TokenType::RIGHT_PAREN))
            {
                exit(1);
            }
        }
        else exit(1);

        std::list<Node*> body;
        if(match(TokenType::LEFT_CUR_BRAC))
        {
            Node* stmt;
            while(!match(TokenType::RIGHT_CUR_BRAC))
            {
                stmt = parseStatement();
                if(!stmt) exit(1);
                body.push_back(stmt);
            }
        }
        else if(!match(TokenType::SEMI_COLON))
        {
            Node* stmt = parseStatement();
            if(!stmt) exit(1);
            body.push_back(stmt);
        }

        WhileNode* temp = new WhileNode(cond, body);
        return temp;
    }
    return nullptr;
}

DoWhileNode* Parser::parseDoWhile()
{
    if(match(TokenType::DO))
    {
        std::list<Node*> body;
        if(match(TokenType::LEFT_CUR_BRAC))
        {
            Node* stmt;
            while(!match(TokenType::RIGHT_CUR_BRAC))
            {
                stmt = parseStatement();
                if(!stmt) exit(1);
                body.push_back(stmt);
            }
        }
        else if(!match(TokenType::SEMI_COLON))
        {
            // TO DO need to check that stmt is not a while node with no body
            Node* stmt = parseStatement();
            if(!stmt) exit(1);
            body.push_back(stmt);
        }

        ExpressionNode* cond;
        if(match(TokenType::WHILE) && match(TokenType::LEFT_PAREN))
        {
            cond = parseExpression();
            if(!cond) exit(1);

            if(match(TokenType::RIGHT_PAREN) && match(TokenType::SEMI_COLON))
            {
                DoWhileNode* temp = new DoWhileNode(cond, body);
                return temp;
            }
        }
        exit(1);
    }
    return nullptr;
}

ForNode* Parser::parseFor()
{
    if(match(TokenType::FOR))
    {   
        if(!match(TokenType::LEFT_PAREN)) exit(1);

        ExpressionNode *init = nullptr, *cond = nullptr, *iter = nullptr;
        if(!match(TokenType::SEMI_COLON))
        {
            init = parseExpression();
            if(!match(TokenType::SEMI_COLON))
            {
                exit(1);
            }
        }

        if(!match(TokenType::SEMI_COLON))
        {
            cond = parseExpression();
            if(!match(TokenType::SEMI_COLON))
            {
                exit(1);
            }
        }

        if(!match(TokenType::RIGHT_PAREN))
        {
            iter = parseExpression();
            if(!match(TokenType::RIGHT_PAREN))
            {
                exit(1);
            }
        }

        std::list<Node*> body;
        if(match(TokenType::LEFT_CUR_BRAC))
        {
            Node* stmt;
            while(!match(TokenType::RIGHT_CUR_BRAC))
            {
                stmt = parseStatement();
                if(!stmt) exit(1);
                body.push_back(stmt);
            }
        }
        else if(!match(TokenType::SEMI_COLON))
        {
            // TO DO need to check that stmt is not a while node with no body
            Node* stmt = parseStatement();
            if(!stmt) exit(1);
            body.push_back(stmt);
        }

        ForNode* temp = new ForNode(init, cond, iter, body);
        return temp;
    }
    return nullptr;
}

SwitchNode* Parser::parseSwitch()
{
    if(match(TokenType::SWITCH))
    {
        ExpressionNode *cond;
        if(match(TokenType::LEFT_PAREN))
        {
            cond = parseExpression();
            if(!cond || !match(TokenType::RIGHT_PAREN))
            {
                exit(1);
            }
        }
        else exit(1);

        std::list<CaseNode*> cases;
        if(match(TokenType::LEFT_CUR_BRAC))
        {
            CaseNode* stmt = nullptr;
            while(!match(TokenType::RIGHT_CUR_BRAC))
            {
                Token val;
                std::list<Node*> body;
                if(match(TokenType::CASE))
                {
                    // TO DO: should check for a constExpr
                    if(match(TokenType::NUMBER) || match(TokenType::STRING) || match(TokenType::TRUE) ||
                         match(TokenType::FALSE) || match(TokenType::IDENTIFIER))
                    {
                        val = prev();
                    }
                    else exit(1);

                    if(!match(TokenType::COLON)) exit(1);

                    while(!match(TokenType::CASE) && !match(TokenType::DEFAULT) && !match(TokenType::RIGHT_CUR_BRAC))
                    {
                        Node* temp = parseStatement();
                        if(!temp) exit(1);
                        body.push_back(temp);
                    }
                    stmt = new CaseNode(val, body);
                    goBack(1);
                }
                else if(match(TokenType::DEFAULT))
                {
                    val = prev();
                    if(!match(TokenType::COLON)) exit(1);
                    while(!match(TokenType::CASE) && !match(TokenType::DEFAULT) && !match(TokenType::RIGHT_CUR_BRAC))
                    {
                        Node* temp = parseStatement();
                        if(!temp) exit(1);
                        body.push_back(temp);
                    }
                    stmt = new CaseNode(val, body);
                    goBack(1);
                }
                //stmt = parseStatement();
                //if(!stmt) exit(1);
                cases.push_back(stmt);
            }
        }
        else if(!match(TokenType::SEMI_COLON))
        {
            // TO DO need to check that stmt is not a while node with no body
            Node* stmt = parseStatement();
            if(!stmt) exit(1);
            Token tkn = {TokenType::DEFAULT, 0, "", 0.0};
            std::list<Node*> body;
            body.push_back(stmt);
            CaseNode* oneStmt = new CaseNode(tkn, body);
            cases.push_back(oneStmt);
        }

        SwitchNode *temp = new SwitchNode(cond, cases);
        return temp;
    }
    return nullptr;
}

ExpressionNode* Parser::parseIdentifier()
{
    ExpressionNode* lhs = nullptr;
    if(match(TokenType::IDENTIFIER))
    {
        lhs = new ExpressionNode(prev(), nullptr, nullptr);
        if(match(TokenType::LEFT_PAREN))
        {
            ExpressionNode* rhs = parseComma();
            if(!match(TokenType::RIGHT_PAREN))
            {
                exit(1);
            }
            Token op{TokenType::FUNC_CALL};
            lhs = new ExpressionNode(op, lhs, rhs);
        }
    }
    return lhs;
}

ExpressionNode* Parser::parseLiteral()
{
    ExpressionNode* lhs = nullptr;
    if(match(TokenType::NUMBER) || match(TokenType::STRING) || 
        match(TokenType::TRUE) || match(TokenType::FALSE))
    {
        lhs = new ExpressionNode(prev(), nullptr, nullptr);
    }
    return lhs;
}

ExpressionNode* Parser::parseScopeRes()
{
    // no brackets allowed
    ExpressionNode* lhs = nullptr;
    if(lhs = parseIdentifier())
    {
        while(match(TokenType::SCOPE))
        {
            Token op = prev();
            ExpressionNode* rhs = parseIdentifier();
            lhs = new ExpressionNode(op, lhs, rhs); 
        }
    }
    else
    {
        lhs = parseLiteral();
    }
    return lhs;
}

ExpressionNode* Parser::parseUnaryPostfix()
{
    ExpressionNode* lhs = parseScopeRes();
    if(match(TokenType::PLUS_PLUS) || match(TokenType::MINUS_MINUS))
    {
        Token op = prev();
        lhs = new ExpressionNode(op, lhs, nullptr);
    }
    //parse funcCall, arrIndexing, member access . ->
    return lhs;
}

ExpressionNode* Parser::parseUnaryPrefix()
{
    ExpressionNode* lhs = nullptr;
    if(match(TokenType::PLUS_PLUS) || match(TokenType::MINUS_MINUS) || match(TokenType::PLUS) ||
        match(TokenType::MINUS) || match(TokenType::EXCLAMATION_MARK) || match(TokenType::BITWISE_NOT))// ||
        //match(TokenType::MULTIPLICATION) || match(TokenType::BITWISE_AND))
    {
        Token op = prev();
        ExpressionNode* rhs = parseUnaryPrefix();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    else 
    {
        lhs = parseUnaryPostfix();
    }
    return lhs;
}

ExpressionNode* Parser::parsePtrToMember()
{
    // ExpressionNode* lhs = parseUnaryPrefix();
    // while(match(TokenType::BITWISE_OR))
    // {
    //     Token op = prev();
    //     ExpressionNode* rhs = parseUnaryPrefix();
    //     lhs = new ExpressionNode(op, lhs, rhs);
    // }
    // return lhs;
    return parseUnaryPrefix();
}

ExpressionNode* Parser::parseProd()
{
    ExpressionNode* lhs = parsePtrToMember();
    while(match(TokenType::MULTIPLICATION) || match(TokenType::DIVIDE) || 
        match(TokenType::MODULO))
    {
        Token op = prev();
        ExpressionNode* rhs = parsePtrToMember();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseSum()
{
    ExpressionNode* lhs = parseProd();
    while(match(TokenType::PLUS) || match(TokenType::MINUS))
    {
        Token op = prev();
        ExpressionNode* rhs = parseProd();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseBitShift()
{
    ExpressionNode* lhs = parseSum();
    while(match(TokenType::LEFT_BITWISE_SHIFT) || match(TokenType::RIGHT_BITWISE_SHIFT))
    {
        Token op = prev();
        ExpressionNode* rhs = parseSum();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseRelational()
{
    ExpressionNode* lhs = parseBitShift();
    while(match(TokenType::LESSER) || match(TokenType::GREATER) || 
        match(TokenType::LESS_EQUAL) || match(TokenType::GREATER_EQUAL))
    {
        Token op = prev();
        ExpressionNode* rhs = parseBitShift();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseEquality()
{
    ExpressionNode* lhs = parseRelational();
    while(match(TokenType::EQUAL) || match(TokenType::NOT_EQUAL))
    {
        Token op = prev();
        ExpressionNode* rhs = parseRelational();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseBitAnd()
{
    ExpressionNode* lhs = parseEquality();
    while(match(TokenType::BITWISE_AND))
    {
        Token op = prev();
        ExpressionNode* rhs = parseEquality();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseBitXor()
{
    ExpressionNode* lhs = parseBitAnd();
    while(match(TokenType::BITWISE_XOR))
    {
        Token op = prev();
        ExpressionNode* rhs = parseBitAnd();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseBitOr()
{
    ExpressionNode* lhs = parseBitXor();
    while(match(TokenType::BITWISE_OR))
    {
        Token op = prev();
        ExpressionNode* rhs = parseBitXor();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseAnd()
{
    ExpressionNode* lhs = parseBitOr();
    while(match(TokenType::LOGIC_AND) || match(TokenType::AND))
    {
        Token op = prev();
        ExpressionNode* rhs = parseBitOr();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseOr()
{
    ExpressionNode* lhs = parseAnd();
    while(match(TokenType::LOGIC_OR) || match(TokenType::OR))
    {
        Token op = prev();
        ExpressionNode* rhs = parseAnd();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseAssignment()
{
    ExpressionNode* lhs = parseOr();
    if(match(TokenType::ASSIGN) || match(TokenType::PLUS_EQUAL) || match(TokenType::MINUS_EQUAL) ||
            match(TokenType::MULT_EQUAL) || match(TokenType::DIV_EQUAL) || match(TokenType::MOD_EQUAL) ||
            match(TokenType::BIT_AND_EQUAL) || match(TokenType::BIT_OR_EQUAL) || match(TokenType::BIT_XOR_EQUAL) ||
            match(TokenType::LEFT_SHIFT_EQUAL) || match(TokenType::RIGHT_SHIFT_EQUAL))
    {
        Token op = prev();
        ExpressionNode* rhs = parseAssignment();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseComma()
{
    ExpressionNode* lhs = parseAssignment();
    while(match(TokenType::COMMA))
    {
        Token op = prev();
        ExpressionNode* rhs = parseAssignment();
        lhs = new ExpressionNode(op, lhs, rhs);
    }
    return lhs;
}

ExpressionNode* Parser::parseExpression()
{
    if(match(TokenType::BREAK) || match(TokenType::CONTINUE))
    {
        return new ExpressionNode(prev(), nullptr, nullptr);
    }
    if(match(TokenType::RETURN))
    {
        return new ExpressionNode(prev(), nullptr, parseComma());
    }

    return parseComma();
}

ExpressionNode* Parser::parseExpressionStatement()
{
    ExpressionNode* node = parseExpression();
    if(match(TokenType::SEMI_COLON))
    {
        return node;
    }
    return nullptr;
}

Node* Parser::parseTopStatement()
{
    Node* temp;
    if(temp = parseClass()) return temp;
    if(temp = parseStruct()) return temp;
    if(temp = parseEnum()) return temp;
    if(temp = parseFunction()) return temp;
    if(temp = parseVariableDeclList()) return temp;
    // TO DO: typedef, namespace, using decls
    return nullptr;
}

Node* Parser::parseStatement()
{
    Node* temp;
    if(temp = parseClass()) return temp;
    if(temp = parseStruct()) return temp;
    if(temp = parseEnum()) return temp;
    if(temp = parseIf()) return temp;
    if(temp = parseWhile()) return temp;
    if(temp = parseDoWhile()) return temp;
    if(temp = parseFor()) return temp;
    if(temp = parseSwitch()) return temp;
    if(temp = parseVariableDeclList()) return temp;
    if(temp = parseExpressionStatement()) return temp;
    // TO DO: typedef, using decls
    return nullptr;
}

Token Parser::prev()
{
    return m_Tokens[m_Current-1];
}

void Parser::goBack(unsigned n)
{
    m_Current -= n;
}

Parser::Parser(const std::vector<Token>& tokens) : 
    m_Tokens(tokens), m_Current(0)
{}

AST Parser::parse()
{
    AST ast;
    while(!match(TokenType::Eof))
    {
        Node* ts = parseTopStatement();
        ast.topStatements.push_back(ts);
    }
    return ast;
}
