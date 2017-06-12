#include "calc.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace calc
{

using it_t = std::string::size_type;

enum class Tok_id
{
    mp = 0,
    div = 1,
    add = 2,
    sub = 3,
    p_left = 4,
    p_right = 5,
    num = 6,
    err = 7
};

struct Token
{
    Tok_id id;
    // precedence for operators
    // value for number
    int data;
};

// data is set only for: + - / *
Token get_token(char c);

std::vector<Token> get_tokens(std::string expr);

std::vector<Token> get_postfix(const std::vector<Token>& tokens);

std::optional<int> postfix_resolve(const std::vector<Token>& tokens);

// I M P L E M E N T A T I O N

void start()
{
    std::string input;
    for(;;)
    {
        std::getline(std::cin, input);
        if(input == "exit")
            return;
        auto val = execute(input);
        if(val)
            std::cout << *val << std::endl;
        else
            std::cout << "bad input, try again." << std::endl;
    }
}

std::optional<int> execute(const std::string& input)
{
    auto tokens = get_tokens(input);
    tokens = get_postfix(tokens);
    if(auto val = postfix_resolve(tokens))
        return *val;
    return {};
}

std::vector<Token> get_tokens(std::string expr)
{
    expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());

    // prevent "6()" from passing
    for(it_t i = 0; i < expr.size() - 1; ++i)
        if(expr[i] == '(' && expr[i + 1]  == ')')
            return {};

    std::vector<Token> tokens;
    tokens.reserve(expr.size());
    for(it_t i = 0; i < expr.size(); ++i)
    {
        auto tok = get_token(expr[i]);
        tokens.push_back(tok);
        // #1 character sequence to single token with id = num conversion
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
        // #2
        //      "-("
        //      "+("
        //   convertion to (token sequence)
        //      -1 *
        //       1 *
        //   or
        //      + -1 *
        //      + 1 *
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

std::vector<Token> get_postfix(const std::vector<Token>& tokens)
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
            // parentheses mismatch
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

std::optional<int> postfix_resolve(const std::vector<Token>& tokens)
{
    // numbers stack
    std::vector<int> num_stack;
    num_stack.reserve(tokens.size());
    for(auto& tok: tokens)
    {
        if(tok.id == Tok_id::num)
            num_stack.push_back(tok.data);
        else
        {
            if(num_stack.size() < 2)
                return {};

            auto top = num_stack.back();
            num_stack.pop_back();
            int val;
            {
                if(tok.id == Tok_id::add)
                    val = num_stack.back() + top;

                else if(tok.id == Tok_id::sub)
                    val = num_stack.back() - top;

                else if(tok.id == Tok_id::div)
                {
                    if(top == 0)
                        return {};
                    val = num_stack.back() / top;
                }
                else if(tok.id == Tok_id::mp)
                    val = num_stack.back() * top;

                else
                    return {};
            }
            num_stack.pop_back();
            num_stack.push_back(val);
        }
    }
    if(num_stack.size() == 1)
        return num_stack.front();
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

} // namespace calc
