/* Crymson Territory */

#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <stack>
using namespace std;

const int LPAREN = 103;
const int RPAREN = 102;
const int ADD = 101;
const int SUB = 100;

class Parser {
  private:
    int result;
    stringstream src;
    stack<int> operands;
    stack<int> operators;
    map<char, int> tokenize;
    vector<int> tokens;
    void parse(string expr);
    void simplify_expr();
    int apply_math(int left, int right, int oper);
    void reset();
  public:
    Parser(); // Constructor.
    int eval(string expr); // The only method we will use at a high level.
    void print(); // Used for debugging purposes.
};

Parser::Parser() {
  tokenize['('] = LPAREN;
  tokenize[')'] = RPAREN;
  tokenize['+'] = ADD;
  tokenize['-'] = SUB;
  tokenize['0'] = 0;
  tokenize['1'] = 1;
  tokenize['2'] = 2;
  tokenize['3'] = 3;
  tokenize['4'] = 4;
  tokenize['5'] = 5;
  tokenize['6'] = 6;
  tokenize['7'] = 7;
  tokenize['8'] = 8;
  tokenize['9'] = 9;
}


void Parser::reset() {
  operands = stack<int>();
  operators = stack<int>();
  tokens.clear(); 
}


void Parser::print() {
  cout << "--- debug parser ---" << endl;
  vector<int> nums;
  while (operands.size() > 0) {
    nums.push_back(operands.top());
    operands.pop();
  }

  cout << "operands: ";
  for (auto num : nums) {
    cout << num << ',';
  }
  cout << endl;

  vector<int> ops;
  while (operators.size() > 0) {
    ops.push_back(operators.top());
    operators.pop();
  }

  cout << "operators: ";
  for (auto op : ops) {
    cout << op << ',';
  }
  cout << endl;
  
  cout << "--------------------" << endl;

  reverse(nums.begin(), nums.end());
  for (int num : nums)
    operands.push(num);

  reverse(ops.begin(), ops.end());
  for (int op : ops)
    operators.push(op);
}

void Parser::parse(string expr) {
  // cout << expr << endl;
  char c;
  for (int i = expr.size() - 1; i >= 0; i--) {
    // cout << expr[i];
    c = expr[i];
    tokens.push_back(tokenize[c]);
  }
}

int Parser::eval(string expr) {
  parse(expr);
  for (int token : tokens) {
    // cout << token << ' ';
    if (token == LPAREN) {
      simplify_expr();
      continue;
    }

    if (token == RPAREN || token == ADD || token == SUB) {
      // cout << token << ' ';
      operators.push(token);
      continue;
    }

    // cout << token << ' ';
    operands.push(token);
  }
  // return 0;
  // if (operators.size() != 0 || operands.size() != 1) {
  //   cout << "### Throw condition has been hit! ###" << endl;
  //   print();
  // }
  
  int evaluation = operands.top();
  operands.pop();
  reset();
  // cout << "====>>> EVAL: " << evaluation << " <<<====" << endl;
  return evaluation;
}

void Parser::simplify_expr() {
  int left, right, oper, result;
  while (operators.top() != RPAREN) {
    // print();
    left = operands.top();
    operands.pop();

    right = operands.top();
    operands.pop();

    oper = operators.top();
    operators.pop();

    result = apply_math(left, right, oper);
    operands.push(result);
  }
  operators.pop();
}

int Parser::apply_math(int left, int right, int oper) {
  if (oper == ADD)
    return left + right;
  
  else
    return left - right;
}