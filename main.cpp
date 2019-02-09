#include <iostream>
#include <fstream>
#include <vector>
#include "Lexer.h"
#include "Parser.h"
#include "ASTPrinter.h"
#include "CodeRefactorer.h"

void printTokens(std::vector<Token> &tokens)
{
    for(int i = 0;i<tokens.size();i++)
    {
        std::cout<<" "<< tokens[i].lexeme <<" on line " << tokens[i].line <<std::endl;
    }
}

void rewriteSourceFile(const std::string& path, const std::vector<Token>& tokens, 
    const std::string& code, const std::string& old, const std::string& replacement)
{
    std::ofstream out(path, std::ios::out | std::ios::trunc);
    unsigned codeInd = 0, tokensSize = tokens.size(), oldLen = old.length();
    for(unsigned i = 0; i < tokensSize; ++i)
    {
        while(code[codeInd] == ' ' || code[codeInd] == '\t' || code[codeInd] == '\n')
        {
            out.put(code[codeInd++]);
        }
        if(tokens[i].type == TokenType::IDENTIFIER && tokens[i].lexeme == replacement)
        {
            codeInd += oldLen;
        }
        else
        {
            codeInd += tokens[i].lexeme.length();
        }
        out << tokens[i].lexeme;
    }
    out.close();
}

bool isIdentifierCorrect(const std::string& str)
{
    if(str[0] == '_' || (str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z'))
    {
        unsigned i = 1;
        for(;str[i] != '\0';++i)
        {
            if(str[i] != '_' && (str[i] < 'a' || str[i] > 'z') && 
                (str[i] < 'A' || str[i] > 'Z') && (str[i] < '0' && str[i] > '9'))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void printFound(const CodeRefactorer& ref)
{
    unsigned i = 1;
    for(auto oc : ref.occurances)
    {
        std::cout << i << ") " << ref.toFind << " is defined on line " << oc.defLine << " as a";
        switch(oc.type)
        {
            case CodeRefactorer::Occurance::IDType::CLASS: std::cout << " class\n";break;
            case CodeRefactorer::Occurance::IDType::ENUM: std::cout << "n enum\n";break;
            case CodeRefactorer::Occurance::IDType::ENUMERATOR: std::cout << "n enumerator\n";break;
            case CodeRefactorer::Occurance::IDType::FUNCTION: std::cout << " function\n";break;
            case CodeRefactorer::Occurance::IDType::VARIABLE: std::cout << " variable\n";break;
            case CodeRefactorer::Occurance::IDType::PARAM: std::cout << " function parameter\n";break;
        }

        std::cout << ref.toFind << " is used on lines: ";
        for(auto uses : oc.useLines)
        {
            std::cout << uses << ", ";
        }
        std::cout << "\n\n";
        ++i;
    }
    std::cout << std::endl;
}

void menu()
{
    std::cout << "Enter the path of the source file you want to refactor:\n> ";

    std::string path;
    std::cin >> path;

    std::ifstream in(path.c_str(), std::ios::in);
    while(!in)
    {
        std::cout << "ERROR: The path you entered is incorrect!\nEnter a correct path:\n> ";
        std::cin >> path;
        in.open(path.c_str(), std::ios::in);
    }

    std::string code;
    while(in)
    {
        code.push_back(in.get());
        //std::cout << code.back();
    }
    in.close();
    code.pop_back();

    Tokenizer tokenizer(code.c_str());
    std::vector<Token> tokens;

    if(tokenizer.tokenize(tokens))
    {
        //printTokens(tokens);

        Parser parser(tokens);
        AST ast = parser.parse();

        // ASTPrinter printer;
        // ast.print(printer);

        std::string toFind;
        std::cout << "Enter the identifier you would like to find:\n> ";
        std::cin >> toFind;
        while(!isIdentifierCorrect(toFind))
        {
            std::cout << "ERROR: the word you entered cannot be an identifier!\nEnter a correct identifier:\n> ";
            std::cin >> toFind;
        }

        CodeRefactorer ref(toFind);
        ast.refactor(ref);

        printFound(ref);

        if(ref.occurances.empty()) return;

        std::cout << "Would you like to replace the found identifier? (y/n)\n> ";
        std::string answer;
        std::cin >> answer;

        if(answer != "y") return;

       
        std::cout << "Enter the identifier you wish to replace " << toFind << " with:\n> ";
        std::cin >> answer;
        while(!isIdentifierCorrect(answer))
        {
            std::cout << "ERROR: the word you entered cannot be an identifier!\nEnter a correct identifier:\n> ";
            std::cin >> answer;
        }

        ref.replace(answer, tokens);
        rewriteSourceFile(path, tokens, code, toFind, answer);
    }
}

int main()
{
    menu();
    
    return 0;
}

