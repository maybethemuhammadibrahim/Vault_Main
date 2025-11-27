#include <iostream>
#include <algorithm> 
using namespace std;

class Node
{
public:
    int data;
    Node *lchild;
    Node *rchild;
    int height;

    Node(int val)
    {
        data = val;
        lchild = rchild = nullptr;
        height = 1; // height of a new node
    }
};

class AVL
{
private:
    Node *root;

    int nodeHeight(Node *p)
    {
        if (p == nullptr)
            return 0;
        int hl = p->lchild ? p->lchild->height : 0;
        int hr = p->rchild ? p->rchild->height : 0;
        return max(hl, hr) + 1;
    }

    int balanceFactor(Node *p)
    {
        if (p == nullptr)
            return 0;
        int hl = p->lchild ? p->lchild->height : 0;
        int hr = p->rchild ? p->rchild->height : 0;
        return hl - hr;
    }

    // Corrected logic to handle both Insert and Delete cases
    Node *balance_node(Node *node)
    {
        int bf = balanceFactor(node);

        // LEFT HEAVY (bf == 2)
        if (bf == 2)
        {
            // Check Left Child
            // If Left Child is Left Heavy (1) OR Balanced (0) -> LL Rotation
            // Note: 0 is possible only during deletion (L0 case)
            if (balanceFactor(node->lchild) >= 0)
                return LLRotation(node);
            
            // If Left Child is Right Heavy (-1) -> LR Rotation
            if (balanceFactor(node->lchild) == -1)
                return LRRotation(node);
        }

        // RIGHT HEAVY (bf == -2)
        else if (bf == -2)
        {
            // Check Right Child (You previously checked lchild here by mistake)
            // If Right Child is Right Heavy (-1) OR Balanced (0) -> RR Rotation
            // Note: 0 is possible only during deletion (R0 case)
            if (balanceFactor(node->rchild) <= 0)
                return RRRotation(node);

            // If Right Child is Left Heavy (1) -> RL Rotation
            if (balanceFactor(node->rchild) == 1)
                return RLRotation(node);
        }

        return node;
    }

    Node *LLRotation(Node *p)
    {
        Node *pl = p->lchild;
        Node *plr = pl->rchild;

        pl->rchild = p;
        p->lchild = plr;

        p->height = nodeHeight(p);
        pl->height = nodeHeight(pl);

        if (p == root)
            root = pl;
        return pl;
    }

    Node *RRRotation(Node *p)
    {
        Node *pr = p->rchild;
        Node *prl = pr->lchild;

        pr->lchild = p;
        p->rchild = prl;

        p->height = nodeHeight(p);
        pr->height = nodeHeight(pr);

        if (p == root)
            root = pr;
        return pr;
    }

    Node *LRRotation(Node *p)
    {
        // First rotate the left child (RR)
        p->lchild = RRRotation(p->lchild);

        // Now rotate the parent (LL)
        return LLRotation(p);
    }

    Node *RLRotation(Node *p)
    {
        // First rotate the right child (LL)
        p->rchild = LLRotation(p->rchild);

        // Now rotate the parent (RR)
        return RRRotation(p);
    }

    Node *rInsert(Node *p, int key)
    {
        if (p == nullptr)
            return new Node(key);

        if (key < p->data)
            p->lchild = rInsert(p->lchild, key);
        else if (key > p->data)
            p->rchild = rInsert(p->rchild, key);
        else
            return p; // Duplicate, no insert

        p->height = nodeHeight(p);
        return balance_node(p);
    }

    Node *findMin(Node *root)
    {
        while (root && root->lchild != NULL)
        {
            root = root->lchild;
        }
        return root;
    }

    Node *deleteBST(Node *root, int key)
    {
        if (root == NULL)
            return root;

        if (key < root->data)
        {
            root->lchild = deleteBST(root->lchild, key);
        }
        else if (key > root->data)
        {
            root->rchild = deleteBST(root->rchild, key);
        }
        else
        {
            // Node found
            // Case 1: No child
            if (root->lchild == NULL && root->rchild == NULL)
            {
                delete root;
                return NULL;
            }
            // Case 2: One child
            else if (root->lchild == NULL)
            {
                Node *temp = root->rchild;
                delete root;
                return temp;
            }
            else if (root->rchild == NULL)
            {
                Node *temp = root->lchild;
                delete root;
                return temp;
            }
            // Case 3: Two children
            Node *temp = findMin(root->rchild);
            root->data = temp->data;
            root->rchild = deleteBST(root->rchild, temp->data);
        }

        if (root == NULL)
            return root;

        root->height = nodeHeight(root);
        return balance_node(root);
    }

    void inorder(Node *p)
    {
        if (!p)
            return;
        inorder(p->lchild);
        cout << p->data << " ";
        inorder(p->rchild);
    }

public:
    AVL() { root = nullptr; }

    void insert(int key)
    {
        root = rInsert(root, key);
    }

    void remove(int key) {
        root = deleteBST(root, key);
    }

    void inorder()
    {
        inorder(root);
        cout << "\n";
    }

    Node *getRoot() { return root; }
};

int main()
{
    AVL t1;
    t1.insert(30);
    t1.insert(20);
    t1.insert(10); // triggers LL rotation
    cout << "LL Rotation Test: ";
    t1.inorder();

    AVL t2;
    t2.insert(10);
    t2.insert(20);
    t2.insert(30); // triggers RR rotation
    cout << "RR Rotation Test: ";
    t2.inorder();

    AVL t3;
    t3.insert(30);
    t3.insert(10);
    t3.insert(20); // triggers LR rotation
    cout << "LR Rotation Test: ";
    t3.inorder();

    AVL t4;
    t4.insert(10);
    t4.insert(30);
    t4.insert(20); // triggers RL rotation
    cout << "RL Rotation Test: ";
    t4.inorder();

    cout << "Deletion Test (Delete 30 from t4): ";
    t4.remove(30);
    t4.inorder();

    return 0;
}
