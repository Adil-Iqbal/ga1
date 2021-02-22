#include <ostream>
#include <iostream>
#include <iomanip>
#include <string>
#include "LinkedList.h"
using namespace std;

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

Color::Modifier r(Color::FG_RED);
Color::Modifier g(Color::FG_GREEN);
Color::Modifier d(Color::FG_DEFAULT);

const int C1 = 30;
const int C2 = 10;

Color::Modifier* c = &r;
string evaluation = "FAIL";

void eval(const char* expected, string actual) {
  if (actual == expected) {
    c = &g;
    evaluation = "PASS";
  } else if (actual == "ACTUAL") {
    c = &d;
    evaluation = "EVALUATION";
  } else {
    c = &r;
    evaluation = "FAIL";
  }
  cout << setw(C2) << (*c) << evaluation << d << endl;
}

void print(const char* label, const char* expected, string actual) {
  string slabel(label);
  cout << left << setw(C1) << right << slabel + ":" << setw(C2) << actual << setw(C2) << expected;
  eval(expected, actual);
}

int main() {
  string act("ACTUAL");
  print("TEST", "EXPECTED", act);
  LinkedList<int> nums;
  print("is_empty method (true)", "1", to_string(nums.is_empty()));
  nums.reverse();
  print("reverse method empty list", "", nums.str());
  // nums.pop();
  // print("pop method empty list", "", nums.str());
  nums.push(5);
  print("push method empty list", "5", nums.str());
  nums.pop();
  print("pop method single item", "", nums.str());
  print("remove method empty list", "0", to_string(nums.remove(5)));
  print("remove_node method empty list", "", nums.str());
  nums.push(5);
  nums.reverse();
  print("reverse method single item", "5", nums.str());

  LinkedList<int> nums2;
  nums2.push(7);
  nums2.push(6);
  nums2.push(5);
  nums2.push(4);
  nums2.push(3);
  nums2.push(2);
  nums2.push(1);
  nums2.push(0);
  cout << left;
  print("is_empty method (false)", "0", to_string(nums2.is_empty()));
  print("push method", "01234567", nums2.str());
  nums2.pop();
  print("pop method", "1234567", nums2.str());
  print("Top method return value", "1", to_string(nums2.top()));
  print("Top method unchanged list", "1234567", nums2.str());
  nums2.reverse();
  print("reverse method", "7654321", nums2.str());
  nums2.remove(4);
  print("remove from middle", "765321", nums2.str());
  nums2.remove(7);
  print("remove from start", "65321", nums2.str());
  nums2.remove(1);
  print("remove from end", "6532", nums2.str());
  nums2.remove_node(0);
  print("remove_node from start", "532", nums2.str());
  nums2.push(9);
  nums2.remove_node(2);
  print("remove_node from end", "952", nums2.str());
  nums2.remove_node(1);
  print("remove_node from middle", "92", nums2.str());

  LinkedList<int> nums3;
  nums3.push(4);
  nums3.push(3);
  nums3.push(2);

  LinkedList<int> nums4 = nums2.merge(nums3);
  print("merge full with full", "92234", nums4.str());
  nums2.pop();
  print("merged list is deep copy 1", "92234", nums4.str());
  nums3.pop();
  print("merged list is deep copy 2", "92234", nums4.str());

  LinkedList<int> nums5;
  LinkedList<int> nums6 = nums4.merge(nums5);
  print("merging full with empty", "92234", nums6.str());
  LinkedList<int> nums7 = nums5.merge(nums4);
  print("merging empty with full", "92234", nums7.str());
  LinkedList<int> nums8 = nums5.merge(nums5);
  print("merging empty with empty", "", nums8.str());

  return 0;
}