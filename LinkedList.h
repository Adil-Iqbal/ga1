/* Chep Territory */
/* Note: Any algorithm that is not O(1) must be written RECURSIVELY! */

#include <string>
#include <iostream>
using namespace std;

class EmptyList {};

template<typename T>
struct Node {
  T data;
  Node* next;
};

template<typename T>
class LinkedList {
  private:
    Node<T>* head;
    void recStr(Node<T>* node, string& str); // Recursively goes through list and returns it as a string.
    void recReverse(Node<T>* previous, Node<T>* current); //Recursively reverses list.
    bool recRemove(Node<T>* previous, Node<T>* current, T& item, bool removed); // Searches list recursively. Returns true if an item is removed from the list.
    void recRemoveNode(Node<T>* previous, Node<T>* current, int itemIndex, int index); // Searches list recursively. Removes the node at once the given index is reached. Nothing is returned.
    void copyList(LinkedList<T>& newList, Node<T>* original); // Creates a new LinkedList and does a deep copy of another LinkedList. Returns current node.
  public:
    LinkedList(); // Constructor.
    bool const is_empty(); // Returns true if list is empty.
    T const top(); // Returns head->data. List is not changed.
    void push(const T& item); // Insert at beginning.
    T pop(); // Remove from beginning, returning what was removed.
    string str(); // Returns list as a string, head is first char: "123" (no spaces or extra characters).
    void reverse(); // In-place reversal of list.
    bool remove(T item); // Remove the first occurance of the given object. Returns true if a node was removed.
    void remove_node(int index); // Remove the node at the given index. Nothing is returned.
    LinkedList<T> merge(LinkedList& other); // Merge two lists together (deep copy). Sequence: This list, then other list.
};

template<typename T>
void LinkedList<T>::recStr(Node<T>* node, string& str){ // CS: Must test.
  if(node != nullptr){
    str += to_string(node->data);
    recStr(node->next, str);
  }
}

template<typename T>
void LinkedList<T>::recReverse(Node<T>* previous, Node<T>* current){ // CS: Must test.
  if(current != nullptr){ // This would mean the list was empty or it reached a tail.
    recReverse(current, current->next);
    if(current->next == nullptr){ // Reached the tail so it is now the new head of the list.
      head = current;
    }
    current->next = previous;
  }
}

template<typename T>
bool LinkedList<T>::recRemove(Node<T>* previous, Node<T>* current, T& item, bool removed){ //CS: Must test.
  if(current != nullptr){
    recRemove(current, current->next, item, removed);
    if(current->data == item){ // Item found; node will be removed.
      if(current == head){
        head = current->next;
        delete current;
      }
      else{
        previous->next = current->next;
        delete current;
      }
    removed = true;
    }
  }
  return removed;
}

template<typename T>
void LinkedList<T>::recRemoveNode(Node<T>* previous, Node<T>* current, int itemIndex, int index){ // CS: Must test.
  if(current != nullptr && (index <= itemIndex)){
    recRemoveNode(current, current->next, itemIndex, index+1);
    if(itemIndex == index){ // Item found; node will be removed.
      if(current == head){
        head = current->next;
        delete current;
      }
      else{
        previous->next = current->next;
        delete current;
      }
    }
  }
}

template<typename T>
void LinkedList<T>::copyList(LinkedList<T>& newList, Node<T>* original){ // CS: Must test.
  if(original != nullptr){
    copyList(newList, original->next);
    newList.push(original->data);
  }
}

template<typename T>
LinkedList<T>::LinkedList() {
  head = nullptr;
}

template<typename T>
bool const LinkedList<T>::is_empty() {
  return head == nullptr;
}

template<typename T>
T const LinkedList<T>::top() {
  if (is_empty())
    throw EmptyList();

  return head->data;
}

template<typename T>
void LinkedList<T>::push(const T& item) {
  Node<T>* newNode = new Node<T>;
  newNode->data = item;
  newNode->next = head;
  head = newNode;
}

template<typename T>
T LinkedList<T>::pop() {
  if (is_empty())
    throw EmptyList();

  T oldHeadData = head->data;
  Node<T>* newHead = head->next;
  delete head;
  head = newHead;
  return oldHeadData;
}

template<typename T>
string LinkedList<T>::str() {
  string s = "";
  recStr(head, s);
  return s;
}

template<typename T>
void LinkedList<T>::reverse() {
  recReverse(nullptr, head);
}

template<typename T>
bool LinkedList<T>::remove(T item) {
  return recRemove(nullptr, head, item, false);
}

template<typename T>
void LinkedList<T>::remove_node(int index) {
  recRemoveNode(nullptr, head, index, 0);
}

template<typename T>
LinkedList<T> LinkedList<T>::merge(LinkedList& other) {
  LinkedList<T> newList;
  copyList(newList, other.head);
  copyList(newList, head);
  return newList;
}