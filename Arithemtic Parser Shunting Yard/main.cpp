#include <iostream>
#include <stack>
#include <unordered_map>
#include <list>
#include <string>
#include <vector>

using namespace std;

const int LEFT_ASSOC = 0;
// const int RIGHT_ASSOC = 1;

unordered_map<string, pair<int, int>> operationsMap;

void initOperationsMap()
{
    operationsMap["+"] = make_pair(0, LEFT_ASSOC);
    operationsMap["-"] = make_pair(0, LEFT_ASSOC);
    operationsMap["*"] = make_pair(1, LEFT_ASSOC);
    operationsMap["/"] = make_pair(1, LEFT_ASSOC);
}

bool isParenthesis(const string &token)
{
    return token == "(" || token == ")";
}

bool isOperator(const string &token)
{
    return operationsMap.find(token) != operationsMap.end();
}

bool isAssociative(const string &token, const int &type)
{
    return operationsMap[token].second == type;
}

int comparePrecedence(const string &token1, const string &token2)
{

    return operationsMap[token1].first - operationsMap[token2].first;
}

/// Convert infix expression format into reverse Polish notation
bool infixToRPN(const vector<string> &inputTokens, vector<string> &strArray)
{
    bool success = true;

    list<string> out;
    stack<string> stack;

    for (string token : inputTokens)
    {
        if (isOperator(token) && !stack.empty())
        {
            string token2 = stack.top();

            while (isOperator(token2) && ((isAssociative(token, LEFT_ASSOC) && comparePrecedence(token, token2) == 0) || (comparePrecedence(token, token2) < 0)))
            {
                stack.pop();
                out.push_back(token2);

                if (!stack.empty())
                    token2 = stack.top();
                else
                    break;
            }
        }

        if (token == "(" || isOperator(token))
        {
            stack.push(token);
        }
        else if (token == ")")
        {
            string topToken = stack.top();

            while (topToken != "(")
            {
                out.push_back(topToken);
                stack.pop();

                if (stack.empty())
                    break;
                topToken = stack.top();
            }

            if (!stack.empty())
                stack.pop();

            if (topToken != "(")
                return false;
        }
        else
        {
            out.push_back(token);
        }
    }

    while (!stack.empty())
    {
        const string stackToken = stack.top();

        if (isParenthesis(stackToken))
            return false;

        out.push_back(stackToken);
        stack.pop();
    }

    strArray.assign(out.begin(), out.end());

    return success;
}

double evaluateRPN(vector<string> tokens)
{
    stack<string> st;

    for (int i = 0; i < (int)tokens.size(); ++i)
    {
        string token = tokens[i];

        if (!isOperator(token))
        {
            st.push(token);
        }
        else
        {
            double result = 0.0;

            string val2 = st.top();
            st.pop();
            double d2 = strtod(val2.c_str(), NULL);

            if (!st.empty())
            {
                string val1 = st.top();
                st.pop();
                double d1 = strtod(val1.c_str(), NULL);

                result = token == "+" ? d1 + d2 : token == "-" ? d1 - d2 : token == "*" ? d1 * d2 : d1 / d2;
            }
            else
            {
                if (token == "-")
                    result = d2 * -1;
                else
                    result = d2;
            }

            st.push(to_string(result));
        }
    }

    return strtod(st.top().c_str(), NULL);
}

vector<string> parseTokens(const string &expression)
{
    vector<string> tokens;

    string tempBuild = "";

    for (char pos : expression)
    {
        string token = string(1, pos);

        if (isOperator(token) || isParenthesis(token))
        {
            if (!tempBuild.empty())
                tokens.push_back(tempBuild);

            tempBuild = "";
            tokens.push_back(token);
        }
        else if (!token.empty() && token != " ")
        {
            // Append the numbers
            tempBuild.append(token);
        }
        else if (tempBuild != "")
        {
            tokens.push_back(tempBuild);
            tempBuild = "";
        }
    }

    tokens.push_back(tempBuild);
    return tokens;
}

int main(int argc, const char *argv[])
{

    initOperationsMap();

    string s;
    getline(cin, s);

    // Tokenize input expression
    vector<string> tokens = parseTokens(s);

    // Evaluate feasible expressions
    vector<string> rpn;
    if (infixToRPN(tokens, rpn))
    {
        double d = evaluateRPN(rpn);

        cout << d << endl;
    }
    else
    {
        cout << "Invalid Input" << endl;
    }

    return 0;
}