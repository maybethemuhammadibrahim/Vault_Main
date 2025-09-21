#include <iostream>
using namespace std;

class SinglyLinkedList {
private:
  struct Node {
    int data;
    Node *next;
    Node(int d) : data(d), next(nullptr) {}
  };

  Node *head;

public:
  SinglyLinkedList() : head(nullptr) {}

  ~SinglyLinkedList() {
    Node *cur = head;
    while (cur) {
      Node *tmp = cur;
      cur = cur->next;
      delete tmp;
    }
  }

  void insertAtStart(int value) {
    Node *n = new Node(value);
    n->next = head;
    head = n;
  }

  void insertAtEnd(int value) {
    Node *n = new Node(value);
    if (!head) {
      head = n;
      return;
    }
    Node *cur = head;
    while (cur->next)
      cur = cur->next;
    cur->next = n;
  }

  // Insert at middle using slow/fast (after first middle in even-sized list)
  void insertAtMiddle(int value) {
    Node *n = new Node(value);
    if (!head) {
      head = n;
      return;
    }
    if (!head->next) {
      head->next = n;
      return;
    }
    Node *slow = head;
    Node *fast = head;
    while (fast->next && fast->next->next) {
      slow = slow->next;
      fast = fast->next->next;
    }
    n->next = slow->next;
    slow->next = n;
  }

  bool deleteByVal(int value) {
    if (!head)
      return false;
    if (head->data == value) {
      Node *tmp = head;
      head = head->next;
      delete tmp;
      return true;
    }
    Node *cur = head;
    while (cur->next && cur->next->data != value) {
      cur = cur->next;
    }
    if (!cur->next)
      return false;
    Node *del = cur->next;
    cur->next = del->next;
    delete del;
    return true;
  }

  void swapNodes(int x, int y) {
    if (x == y) {
      return; // No need to swap identical values
    }

    // Search for x (keep track of prevX and currX)
    Node *prevX = nullptr, *currX = head;
    while (currX && currX->data != x) {
      prevX = currX;
      currX = currX->next;
    }

    // Search for y (keep track of prevY and currY)
    Node *prevY = nullptr, *currY = head;
    while (currY && currY->data != y) {
      prevY = currY;
      currY = currY->next;
    }

    // If either x or y is not present, do nothing
    if (!currX || !currY) {
      cout << "One or both values not found in the list.\n";
      return;
    }

    // If x is not at head, update prevX's next, otherwise update head.
    if (prevX)
      prevX->next = currY;
    else
      head = currY;

    // Similarly for y
    if (prevY)
      prevY->next = currX;
    else
      head = currX;

    // Swap next pointers of currX and currY
    Node *temp = currX->next;
    currX->next = currY->next;
    currY->next = temp;
  }

  void reverseLL(int length) {
    Node *currentH = head;
    Node *currentT = head;
    int temp;
    for (int i = 0; i < length / 2; i++) {
      currentT = head;
      for (int j = 0; j < length - i - 1; j++) {
        currentT = currentT->next;
      }

      temp = currentT->data;
      currentT->data = currentH->data;
      currentH->data = temp;

      currentH = currentH->next;
    }
  }

  void removeDoubles() {
    Node *outer = head;
    Node *inner = nullptr;
    Node *innerPrev = nullptr;
    Node *temp;
    while (outer->next != nullptr) {
      inner = outer->next;
      innerPrev = outer;
      while (inner != nullptr) {
        if (inner == nullptr)
          break;
        if (outer->data == inner->data) {
          temp = inner;
          innerPrev->next = inner->next;
          delete inner;
          inner = innerPrev->next;
        } else {
          innerPrev = inner;
          inner = inner->next;
        }
      }
      outer = outer->next;
    }
  }

  void print() const {
    Node *cur = head;
    while (cur) {
      cout << cur->data;
      if (cur->next)
        cout << " -> ";
      cur = cur->next;
    }
    cout << "\n";
  }

  bool empty() const { return head == nullptr; }

private:
  int length() const {
    int count = 0;
    Node *cur = head;
    while (cur) {
      ++count;
      cur = cur->next;
    }
    return count;
  }

  Node *nodeAtIndex(int index) const {
    Node *cur = head;
    while (index-- && cur)
      cur = cur->next;
    return cur;
  }
};

int main() {
  SinglyLinkedList list;
  list.insertAtEnd(10);
  list.insertAtEnd(20);
  list.insertAtEnd(30);
  list.insertAtStart(5);
  list.insertAtMiddle(25);
  cout << "After insertions:\n";
  list.print();

  list.deleteByVal(20);
  cout << "After delete 20:\n";
  list.print();

  list.insertAtEnd(30);
  list.insertAtEnd(10);
  list.insertAtEnd(5);
  cout << "With duplicates:\n";
  list.print();

  list.removeDoubles();
  cout << "After removing duplicates:\n";
  list.print();

  list.swapNodes(5, 30);
  cout << "After swapping 5 and 30:\n";
  list.print();

  list.reverseLL(5);
  cout << "After reverse (data swap only):\n";
  list.print();

  return 0;
}