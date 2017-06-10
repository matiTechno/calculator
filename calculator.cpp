#include "calculator.hpp"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>

// don't change the order
enum class Tok_id
{
    mp,
    div,
    add,
    sub,
    p_left,
    p_right,
    err,
    num
};

struct Token
{
    Tok_id id;
    // precedence for operators
    // value for number
    int data;
};

// sets data only for + - * / operators
Token get_token(char c);

// expr must be whitespace free
// does some processing
std::vector<Token> get_tokens(const std::string &expr);

// output token list consist only of numbers and + - * / operators
std::optional<std::vector<Token>> make_postfix(const std::vector<Token> &tokens);

std::optional<int> postfix_resolve(const std::vector<Token> &tokens);

void calc_start()
{
    std::string input;
    for(;;)
    {
        std::getline(std::cin, input);
        if(input == "exit")
            return;
        auto val = calc_execute(input);
        if(val)
            std::cout << *val << std::endl;
        else
            std::cout << "bad input, try again." << std::endl;
    }
}

std::optional<int> calc_execute(std::string input)
{
    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    auto tokens = get_tokens(input);
    auto ptokens = make_postfix(tokens);

    if(!ptokens)
        return {};

    if(auto val = postfix_resolve(*ptokens))
        return *val;

    return {};
}

std::vector<Token> get_tokens(const std::string &expr)
{
    for(auto c: expr)
        assert(c != ' ');

    std::vector<Token> tokens;
    tokens.reserve(expr.size());
    for(auto i = 0; i < expr.size(); ++i)
    {
        auto tok = get_token(expr[i]);
        tokens.push_back(tok);
        if(
                tok.id == Tok_id::num
                ||
                (
                    (tok.id == Tok_id::add || tok.id == Tok_id::sub)
                    &&
                    i < expr.size() - 1
                    &&
                    get_token(expr[i + 1]).id == Tok_id::num
                    &&
                    (i == 0 || get_token(expr[i - 1]).id == Tok_id::p_left)
                    )
                )
        {
            auto end = i + 1;
            while(end < expr.size())
            {
                if(get_token(expr[end]).id != Tok_id::num)
                    break;
                ++end;
            }
            tokens.back() = {Tok_id::num, std::stoi(std::string(expr, i, end - i))};
            i = end - 1;
        }
        else if(
                (tok.id == Tok_id::add || tok.id == Tok_id::sub)
                &&
                i < expr.size() - 1
                &&
                get_token(expr[i + 1]).id == Tok_id::p_left
                )
        {
            if(tok.id == Tok_id::add)
                tokens.back() = {Tok_id::num, 1};
            else
                tokens.back() = {Tok_id::num, -1};

            if(i > 0 && get_token(expr[i - 1]).id != Tok_id::p_left)
                tokens.insert(tokens.end() - 1, get_token('+'));

            tokens.push_back(get_token('*'));
        }
    }
    return tokens;
}

std::optional<std::vector<Token>> make_postfix(const std::vector<Token> &tokens)
{
    std::vector<Token> ptokens;
    // operators stack
    std::vector<Token> op_stack;
    ptokens.reserve(tokens.size());
    op_stack.reserve(tokens.size());

    for(auto& tok: tokens)
    {
        if(tok.id == Tok_id::num)
            ptokens.push_back(tok);
        else if(tok.id <= Tok_id::sub)
        {
            if(op_stack.size() && op_stack.back().id != Tok_id::p_left)
            {
                int erase_id = -1;
                for(int i = op_stack.size() - 1; i != -1; --i)
                {
                    if(op_stack[i].data >= tok.data)
                    {
                        ptokens.push_back(op_stack[i]);
                        erase_id = i;
                    }
                    else
                        break;
                }
                if(erase_id != -1)
                    op_stack.erase(op_stack.begin() + erase_id, op_stack.end());
            }
            op_stack.push_back(tok);
        }
        else if(tok.id == Tok_id::p_left)
            op_stack.push_back(tok);

        else if(tok.id == Tok_id::p_right)
        {
            if(op_stack.empty())
                return {};

            int erase_id;
            for(int i = op_stack.size() - 1; i != -1; --i)
            {
                erase_id = i;
                if(op_stack[i].id == Tok_id::p_left)
                    break;
                ptokens.push_back(op_stack[i]);
            }
            // missing (
            if(erase_id == 0 && op_stack[0].id != Tok_id::p_left)
                return {};

            op_stack.erase(op_stack.begin() + erase_id, op_stack.end());
        }
        else
            return {};
    }
    for(auto it = op_stack.rbegin(); it != op_stack.rend(); ++it)
    {
        if(it->id == Tok_id::p_left)
            return {};
        ptokens.push_back(*it);
    }
    return ptokens;
}

std::optional<int> postfix_resolve(const std::vector<Token> &tokens)
{
    // operands stack
    std::vector<int> op_stack;
    op_stack.reserve(tokens.size());
    for(auto& tok: tokens)
    {
        if(tok.id == Tok_id::num)
            op_stack.push_back(tok.data);
        else
        {
            if(op_stack.size() < 2)
                return {};

            auto top = op_stack.back();
            op_stack.pop_back();
            int val;
            switch(tok.id)
            {
            case Tok_id::add:
                val = op_stack.back() + top;
                break;

            case Tok_id::sub:
                val = op_stack.back() - top;
                break;

            case Tok_id::div:
                if(top == 0)
                    return {};
                val = op_stack.back() / top;
                break;

            case Tok_id::mp:
                val = op_stack.back() * top;
            }
            op_stack.pop_back();
            op_stack.push_back(val);
        }
    }
    if(op_stack.size() == 1)
        return op_stack[0];
    return {};
}

Token get_token(char c)
{
    if(c >= '0' && c <= '9')
        return {Tok_id::num};
    if(c == '*')
        return {Tok_id::mp, 3};
    if(c == '/')
        return {Tok_id::div, 3};
    if(c == '+')
        return {Tok_id::add, 2};
    if(c == '-')
        return {Tok_id::sub, 2};
    if(c == '(')
        return {Tok_id::p_left};
    if(c == ')')
        return {Tok_id::p_right};
    else
        return {Tok_id::err};
}
