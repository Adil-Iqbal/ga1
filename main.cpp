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
int eval(string& expr);

int main(int argc, char* argv[]) {
  ArgumentManager am(argc, argv);
  string outFilename = am.get("output");
  string inFilename = am.get("input");
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

  cout << "Sarah: " << arrayFormat(sstr) << endl;
  cout << "Alex: " << arrayFormat(astr) << endl;
  cout << "Decoded passcode: " << codeFormat(jstr) << endl;
  cout << "Actual passcode: " << codeFormat(pstr) << endl;

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
      if (closed_brkts[c] != stack.pop()) 
        return false;
    }
  }
  return stack.is_empty();
};

/* Evaluate Expression */
int eval(string& expr) {
  sanitizeExpression(expr);
  LinkedList<int> nums;
  LinkedList<char> ops;
  int num1, num2;
  char oper;

  for (int i = 0; i < expr.length(); i++) {
    char c = expr[i];
    if (isdigit(c))
      nums.push(c - 48); // ASCII of '0' is 48
    else if (c != '(')
      ops.push(c);
    else {
      while(!ops.is_empty() && ops.top() != ')') {
        oper = ops.pop();
        num1 = nums.pop();
        num2 = nums.pop();

        switch (oper) {
          case '+': nums.push(num1 + num2); break;
          case '-': nums.push(num1 - num2); break;
        }
      }
      oper = ops.pop();
    }
  }

  return nums.pop();
};