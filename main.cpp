#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include "LinkedList.h"
#include "ArgumentManager.h"
using namespace std;

// Function declarations.
bool bracketsBalanced(string& expr);
void sanitizeExpression(string& expr);
string codeFormat(string& src);
string arrayFormat(string& src);
int eval(const string& expr);

int main(int argc, char* argv[]) {
  ArgumentManager am(argc, argv);
  string outFilename = am.get("output");
  string inFilename = am.get("input");
  // string outFilename = "output13.txt";
  // string inFilename = "input13.txt";
  ofstream outFile(outFilename);
  ifstream inFile(inFilename);

  int num;
  bool removed;
  int invalid = 0;
  LinkedList<int> sarah, alex;
  LinkedList<int>* loc = &sarah;
  vector<int> rejected;
  string temp, pstr;

  while(!inFile.eof()) {
    getline(inFile, temp);

    // Ignore blank lines.
    if (temp == "")
      continue;
    
    // Set location of insertion.
    if (temp.rfind("Sarah", 0) == 0) {
      loc = &sarah;
      continue;
    }

    else if (temp.rfind("Alex", 0) == 0) {
      loc = &alex;
      continue;
    }

    // Retrieve passcode.
    else if (temp.rfind("Passcode:", 0) == 0) {
      pstr = temp.substr(9, temp.size() - 9);
      continue;
    }
    
    // Validate expression.
    if (!bracketsBalanced(temp)) {
      invalid += 1;
      continue;
    }

    // Evaluate expression.
    num = eval(temp);

    // Validate evaluation.
    if (num < 0) {
      rejected.push_back(num * -1);
      continue;
    }

    // Insert num into respective stack.
    loc->push(num);
  }

  // Remove indicated values.
  for (int reject : rejected) {
    removed = sarah.remove(reject);
    if (!removed)
      alex.remove(reject);
  }

  // Merge two lists.
  LinkedList<int> jewels = alex.merge(sarah);
  
  // Reverse linked list.
  jewels.reverse();

  // Remove indicated node.
  if (invalid > 0)
    jewels.remove_node(invalid);

  // Print result to output. 
  string sstr = sarah.str();
  string astr = alex.str();
  string jstr = jewels.str();

  outFile << "Sarah: " << arrayFormat(sstr) << endl;
  outFile << "Alex: " << arrayFormat(astr) << endl;
  outFile << "Decoded passcode: " << codeFormat(jstr) << endl;
  outFile << "Actual passcode: " << codeFormat(pstr) << endl;

  return 0;
}

/* Converts string to array format: "abc" --> "[a, b, c]" */
string arrayFormat(string& src) {
  string arr = "[";
  if (src.size() == 0)
    return arr + ']';
  for (int i = 0; i < src.size(); i++) {
    arr += src[i];
    if (i < src.size() - 1) {
      arr += ',';
      arr += ' ';
    } else 
      arr += ']';
  }
  return arr;
}

/* Converts string to code format: "abc" --> "| a | b | c |" */
string codeFormat(string& src) {
  string code = "| ";
  if (src.size() == 0)
    return code + '|';
  for (int i = 0; i < src.size(); i++) {
    code += src[i];
    code += ' ';
    code += '|';
    if (i != src.size() - 1)
      code += ' ';
  }
  return code;
}

/* Transforms expression into easily parsable format. */
const auto open_brkt_regex = regex(R"([\[\{]{1})");
const auto closed_brkt_regex = regex(R"([\]\}]{1})");
const string lparen = "(";
const string rparen = ")";
void sanitizeExpression(string& expr) {
  expr = regex_replace(expr, open_brkt_regex, lparen);
  expr = regex_replace(expr, closed_brkt_regex, rparen);
  expr = lparen + expr + rparen;
}

/* Validates whether expression's parenthesis are balanced. */
map<char, int> open_brkts = {{'(', 1}, {'[', 2}, {'{', 3}};
map<char, int> closed_brkts = {{')', 1}, {']', 2}, {'}', 3}};
bool bracketsBalanced(string& expr) {
  LinkedList<int> stack;
  for (int i = 0; i < expr.size(); i++) {
    char c = expr[i];
    if (open_brkts[c] > 0) {
      stack.push(open_brkts[c]);
      continue;
    }
    else if (closed_brkts[c] > 0) {
      if (stack.is_empty())
        return false;
      if (closed_brkts[c] != stack.top())
        return false;
      else
        stack.pop();
    }
  }
  return stack.is_empty();
};

/* Evaluate Expression */
int eval(const string& raw) {
  string expr = raw;
  sanitizeExpression(expr);
  //CONVERT INFIX TO POSTFIX
  string postfix = "";
  LinkedList<char> ops;

  for (int i = 0; i < expr.length(); i++) {
    if (isdigit(expr[i]))
      postfix.push_back(expr[i]);
    else if (expr[i] == '(')
      ops.push(expr[i]);
    else if (expr[i] == ')') {
      while (!ops.is_empty() && ops.top() != '(') {
        postfix.push_back(ops.top());
        ops.pop();
      }
      if (ops.top() == '(')
        ops.pop();  
    }
    else {
      while (!ops.is_empty() && ops.top() != '(') {
        postfix.push_back(ops.top());
        ops.pop();
      }
      ops.push(expr[i]);
    }
  }
  while (!ops.is_empty()) {
    postfix.push_back(ops.top());
    ops.pop();
  }

  //EVALUATE POSTFIX
  LinkedList<char> nums;
  int num1, num2;
  
  for (int i = 0; i < postfix.length(); i++) {
    if (isdigit(postfix[i]))
      nums.push(postfix[i] - 48);
    else {
      num1 = nums.top();
      nums.pop();
      num2 = nums.top();
      nums.pop();
      switch(postfix[i]) {
        case '+': nums.push(num2 + num1); break;
        case '-': nums.push(num2 - num1); break;
      }
    }
  }
  if (nums.is_empty())
    return 0;
  return nums.top();
};