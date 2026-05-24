#include <iostream>
#include <stack>
#include <string>
using namespace std;

char tokType[200]; 
string tokVal[200];
int tokCount = 0;

char pfxType[200];
string pfxVal[200];
int pfxCount = 0;

string varNames[100];
double varValues[100];
int varCount = 0; 

bool isOpenBracket(char c) { return c == '(' || c == '[' || c == '{'; }
bool isCloseBracket(char c) { return c == ')' || c == ']' || c == '}'; }

bool isMatchingPair(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

int precedence(char op)
{
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}

int strToInt(const string &s)
{
    int result = 0;
    for (int i = 0; i < (int)s.size(); i++)
        result = result * 10 + (s[i] - '0');
    return result;
}

bool strEq(const string &a, const string &b)
{
    if (a.size() != b.size())
        return false;
    for (int i = 0; i < (int)a.size(); i++)
        if (a[i] != b[i])
            return false;
    return true;
}


void tokenize(const string &expr)
{
    int i = 0, n = expr.size();

    while (i < n)
    {
        if (expr[i] == ' ')
        {
            i++;
            continue;
        }

        if (isOpenBracket(expr[i]) || isCloseBracket(expr[i]))
        {
            tokType[tokCount] = isOpenBracket(expr[i]) ? '(' : ')';
            tokVal[tokCount] = string(1, expr[i]);
            tokCount++;
            i++;
        }
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')
        {
            tokType[tokCount] = 'O';
            tokVal[tokCount] = string(1, expr[i]);
            tokCount++;
            i++;
        }
        else if (isdigit(expr[i]))
        {
            string num = "";
            while (i < n && isdigit(expr[i]))
            {
                num += expr[i];
                i++;
            }
            tokType[tokCount] = 'N';
            tokVal[tokCount] = num;
            tokCount++;
        }
        else if (isalpha(expr[i]) || expr[i] == '_')
        {
            string var = "";
            while (i < n && (isalnum(expr[i]) || expr[i] == '_'))
            {
                var += expr[i];
                i++;
            }
            tokType[tokCount] = 'V';
            tokVal[tokCount] = var;
            tokCount++;
        }
        else
        {
            cerr << "Syntax error: unexpected character '" << expr[i] << "'" << endl;
            exit(1);
        }
    }
}


void validate()
{
    if (tokCount == 0)
    {
        cerr << "Syntax error: empty expression" << endl;
        exit(1);
    }
    if (tokType[0] == 'O')
    {
        cerr << "Syntax error: expression starts with operator" << endl;
        exit(1);
    }
    if (tokType[tokCount - 1] == 'O')
    {
        cerr << "Syntax error: expression ends with operator" << endl;
        exit(1);
    }

    for (int i = 0; i < tokCount - 1; i++)
    {
        char a = tokType[i];
        char b = tokType[i + 1];

        if ((a == 'N' || a == 'V') && (b == 'N' || b == 'V'))
        {
            cerr << "Syntax error: missing operator between '"
                 << tokVal[i] << "' and '" << tokVal[i + 1] << "'" << endl;
            exit(1);
        }
        if (a == 'O' && b == 'O')
        {
            cerr << "Syntax error: two operators in a row '"
                 << tokVal[i] << "' '" << tokVal[i + 1] << "'" << endl;
            exit(1);
        }
        if (a == 'O' && b == ')')
        {
            cerr << "Syntax error: operator before closing bracket" << endl;
            exit(1);
        }
        if (a == '(' && b == 'O')
        {
            cerr << "Syntax error: operator right after opening bracket" << endl;
            exit(1);
        }
        if ((a == 'N' || a == 'V') && b == '(')
        {
            cerr << "Syntax error: missing operator before bracket" << endl;
            exit(1);
        }
        if (a == ')' && (b == 'N' || b == 'V'))
        {
            cerr << "Syntax error: missing operator after bracket" << endl;
            exit(1);
        }
    }
}


char opStkType[200];
string opStkVal[200];
int opStkTop = -1;

void toPostfix()
{
    for (int i = 0; i < tokCount; i++)
    {

        if (tokType[i] == 'N' || tokType[i] == 'V')
        {
            pfxType[pfxCount] = tokType[i];
            pfxVal[pfxCount] = tokVal[i];
            pfxCount++;
        }
        else if (tokType[i] == 'O')
        {
            while (opStkTop >= 0 &&
                   opStkType[opStkTop] == 'O' &&
                   precedence(opStkVal[opStkTop][0]) >= precedence(tokVal[i][0]))
            {
                pfxType[pfxCount] = opStkType[opStkTop];
                pfxVal[pfxCount] = opStkVal[opStkTop];
                pfxCount++;
                opStkTop--;
            }
            opStkTop++;
            opStkType[opStkTop] = tokType[i];
            opStkVal[opStkTop] = tokVal[i];
        }
        else if (tokType[i] == '(')
        {
            opStkTop++;
            opStkType[opStkTop] = tokType[i];
            opStkVal[opStkTop] = tokVal[i];
        }
        else if (tokType[i] == ')')
        {
            bool found = false;
            while (opStkTop >= 0 && !found)
            {
                if (opStkType[opStkTop] == '(')
                {
                    if (!isMatchingPair(opStkVal[opStkTop][0], tokVal[i][0]))
                    {
                        cerr << "Syntax error: mismatched brackets '"
                             << opStkVal[opStkTop] << "' and '" << tokVal[i] << "'" << endl;
                        exit(1);
                    }
                    opStkTop--;
                    found = true;
                }
                else
                {
                    pfxType[pfxCount] = opStkType[opStkTop];
                    pfxVal[pfxCount] = opStkVal[opStkTop];
                    pfxCount++;
                    opStkTop--;
                }
            }
            if (!found)
            {
                cerr << "Syntax error: unmatched closing bracket '" << tokVal[i] << "'" << endl;
                exit(1);
            }
        }
    }

    while (opStkTop >= 0)
    {
        if (opStkType[opStkTop] == '(')
        {
            cerr << "Syntax error: unmatched opening bracket '" << opStkVal[opStkTop] << "'" << endl;
            exit(1);
        }
        pfxType[pfxCount] = opStkType[opStkTop];
        pfxVal[pfxCount] = opStkVal[opStkTop];
        pfxCount++;
        opStkTop--;
    }
}


int main()
{
    cerr << "Enter infix expression: ";
    string expr;
    if (!getline(cin, expr))
    {
        cerr << "Syntax error: no input" << endl;
        return 1;
    }

    tokenize(expr);
    validate();
    toPostfix();
    

    for (int i = 0; i < pfxCount; i++)
    {
        if (i > 0)
            cout << " ";
        cout << pfxVal[i];
    }
    cout << "\n";
    

    for (int i = 0; i < tokCount; i++)
    {
        if (tokType[i] == 'V')
        {
            bool found = false;
            for (int j = 0; j < varCount; j++)
            {
                if (strEq(varNames[j], tokVal[i]))
                {
                    found = true;
                }
            }
            if (!found)
            {
                varNames[varCount] = tokVal[i];
                varCount++;
            }
        }
    }

    
    for (int i = 0; i < varCount; i++)
    {
        cerr << "Enter value for " << varNames[i] << ": ";
        if (!(cin >> varValues[i]))
        {
            cerr << "Runtime error: invalid value for '" << varNames[i] << "'" << endl;
            return 2;
        }
    }

    
    double evalStk[200];
    int evalTop = -1;

    for (int i = 0; i < pfxCount; i++)
    {
        if (pfxType[i] == 'N')
        {
            evalTop++;
            evalStk[evalTop] = (double)strToInt(pfxVal[i]);
        }
        else if (pfxType[i] == 'V')
        {
            double val = 0;
            for (int j = 0; j < varCount; j++)
            {
                if (strEq(varNames[j], pfxVal[i]))
                {
                    val = varValues[j];
                }
            }
            evalTop++;
            evalStk[evalTop] = val;
        }
        else if (pfxType[i] == 'O')
        {
            if (evalTop < 1)
            {
                cerr << "Logical error: not enough operands for '" << pfxVal[i] << "'" << endl;
                return 3;
            }
            double b = evalStk[evalTop];
            evalTop--;
            double a = evalStk[evalTop];
            evalTop--;
            double res = 0;
            char op = pfxVal[i][0];

            if (op == '+')
                res = a + b;
            else if (op == '-')
                res = a - b;
            else if (op == '*')
                res = a * b;
            else if (op == '/')
            {
                if (b == 0)
                {
                    cerr << "Runtime error: division by zero" << endl;
                    return 2;
                }
                res = a / b;
            }
            evalStk[evalTop + 1] = res;
            evalTop++;
        }
    }

    if (evalTop != 0)
    {
        cerr << "Logical error: malformed expression" << endl;
        return 3;
    }

    double result = evalStk[0];
    if (result == (long long)result)
        cout << (long long)result << "\n";
    else
        cout << result << "\n";

    
}