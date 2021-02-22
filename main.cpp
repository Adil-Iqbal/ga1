#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include "LinkedList.h"
#include "Parser.h"
#include "ArgumentManager.h"
using namespace std;

// Function declarations.
bool bracketsBalanced(string& expr);
void sanitizeExpression(string& expr);
string codeFormat(string& src);
string arrayFormat(string& src);

int main(int argc, char* argv[]) {
  // ArgumentManager am(argc, argv);
  // string outFilename = am.get("output");
  // string inFilename = am.get("input");

  // ofstream outFile(outFilename);
  // ifstream inFile(inFilename);

  ifstream inFile("input12.txt");

  Parser parser;
  int num;
  bool removed;
  int invalid = 0;
  LinkedList sarah, alex;
  LinkedList* loc = &sarah;
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
    sanitizeExpression(temp);
    num = parser.eval(temp);

    // Validate evaluation.
    if (num < 0) {
      rejected.push_back(num * -1);
      continue;
    }

    // Insert num into respective stack.
    (*loc).push(num);
  }

  // Correct order.
  sarah.reverse();
  alex.reverse();

  // Remove indicated values.
  for (int reject : rejected) {
    removed = sarah.remove(reject);
    if (!removed)
      alex.remove(reject);
  }

  // Merge two lists.
  LinkedList jewels = sarah.merge(alex);

  // Remove indicated node.
  if (invalid > 0)
    jewels.remove_node(invalid);
  
  // Reverse linked list.
  jewels.reverse();

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
  expr = regex_replace(expr, open_brkt_regex, "(");
  expr = regex_replace(expr, closed_brkt_regex, ")");
  expr = lparen + expr + rparen;
}

/* Validates whether expression's parenthesis are balanced. */
const char open_brackets[] = { '(', '[', '{' }; 
const char closed_brackets[] = { ')',  ']', '}' };
bool bracketsBalanced(string& expr) {
  LinkedList stack;
  for (int i = 0; i < expr.size(); i++) {
    for (int j = 0; j < 3; j++) {
      if (expr[i] == open_brackets[j]) {
        stack.push(j);
        break;
      }
      if (expr[i] == closed_brackets[j]) {
        if (stack.is_empty())
          return false;
        if (j == stack.top()) {
          stack.pop();
          break;
        } else 
          return false;
      }
    }
  }
  if (!stack.is_empty())
    return false;
  return true;
};

