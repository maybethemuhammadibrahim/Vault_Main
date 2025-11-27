#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* lchild;
    Node* rchild;
    int height;

    Node(int val) {
        data = val;
        lchild = rchild = nullptr;
        height = 1;    // height of a new node
    }
};

class AVL {
private:
    Node* root;

    int nodeHeight(Node* p) {
        if (p == nullptr) return 0;
        int hl = p->lchild ? p->lchild->height : 0;
        int hr = p->rchild ? p->rchild->height : 0;
        return max(hl, hr) + 1;
    }

    int balanceFactor(Node* p) {
        if (p == nullptr) return 0;
        int hl = p->lchild ? p->lchild->height : 0;
        int hr = p->rchild ? p->rchild->height : 0;
        return hl - hr;
    }

    Node* LLRotation(Node* p) {
        Node* pl  = p->lchild;
        Node* plr = pl->rchild;

        pl->rchild = p;
        p->lchild  = plr;

        p->height  = nodeHeight(p);
        pl->height = nodeHeight(pl);

        if (p == root) root = pl;
        return pl;
    }

    Node* RRRotation(Node* p) {
        Node* pr  = p->rchild;
        Node* prl = pr->lchild;

        pr->lchild = p;
        p->rchild  = prl;

        p->height  = nodeHeight(p);
        pr->height = nodeHeight(pr);

        if (p == root) root = pr;
        return pr;
    }

    Node* LRRotation(Node* p) {
        // First rotate the left child (RR)
        p->lchild = RRRotation(p->lchild);
    
        // Now rotate the parent (LL)
        return LLRotation(p);
    }


    // Node* LRRotation(Node* p) {
    //     Node* pl  = p->lchild;
    //     Node* plr = pl->rchild;

    //     // Step 1: rotate left on left child
    //     pl->rchild = plr->lchild;
    //     plr->lchild = pl;

    //     // Step 2: rotate right on p
    //     p->lchild  = plr->rchild;
    //     plr->rchild = p;

    //     // Update heights
    //     pl->height  = nodeHeight(pl);
    //     p->height   = nodeHeight(p);
    //     plr->height = nodeHeight(plr);

    //     if (p == root) root = plr;
    //     return plr;
    // }

    Node* AVL::RLRotation(Node* p) {
        // First rotate the right child (LL)
        p->rchild = LLRotation(p->rchild);
    
        // Now rotate the parent (RR)
        return RRRotation(p);
    }        


    // Node* RLRotation(Node* p) {
    //     Node* pr  = p->rchild;
    //     Node* prl = pr->lchild;

    //     // Step 1: rotate right on right child
    //     pr->lchild = prl->rchild;
    //     prl->rchild = pr;

    //     // Step 2: rotate left on p
    //     p->rchild  = prl->lchild;
    //     prl->lchild = p;

    //     // Update heights
    //     pr->height  = nodeHeight(pr);
    //     p->height   = nodeHeight(p);
    //     prl->height = nodeHeight(prl);

    //     if (p == root) root = prl;
    //     return prl;
    // }

    Node* rInsert(Node* p, int key) {
        if (p == nullptr) {
            return new Node(key);
        }

        if (key < p->data)
            p->lchild = rInsert(p->lchild, key);
        else if (key > p->data)
            p->rchild = rInsert(p->rchild, key);
        else
            return p; // Duplicate, no insert

        // Update height
        p->height = nodeHeight(p);

        // Balance & rotate
        int bf = balanceFactor(p);

        // LL
        if (bf == 2 && key < p->lchild->data)
            return LLRotation(p);

        // LR
        if (bf == 2 && key > p->lchild->data)
            return LRRotation(p);

        // RR
        if (bf == -2 && key > p->rchild->data)
            return RRRotation(p);

        // RL
        if (bf == -2 && key < p->rchild->data)
            return RLRotation(p);

        return p;
    }

    void inorder(Node* p) {
        if (!p) return;
        inorder(p->lchild);
        cout << p->data << " ";
        inorder(p->rchild);
    }

public:
    AVL() { root = nullptr; }

    void insert(int key) {
        root = rInsert(root, key);
    }

    void inorder() {
        inorder(root);
        cout << "\n";
    }

    Node* getRoot() { return root; }
};

int main() {
    AVL t1;
    t1.insert(30);
    t1.insert(20);
    t1.insert(10);     // triggers LL rotation
    t1.inorder();

    AVL t2;
    t2.insert(10);
    t2.insert(20);
    t2.insert(30);     // triggers RR rotation
    t2.inorder();

    AVL t3;
    t3.insert(30);
    t3.insert(10);
    t3.insert(20);     // triggers LR rotation
    t3.inorder();

    AVL t4;
    t4.insert(10);
    t4.insert(30);
    t4.insert(20);     // triggers RL rotation
    t4.inorder();

    return 0;
}
