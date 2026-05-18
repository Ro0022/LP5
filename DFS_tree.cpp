#include <iostream>
#include <omp.h>

using namespace std;

// Tree Node
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = right = NULL;
    }
};

// Parallel DFS Function
void parallelDFS(Node* root) {

    if (root == NULL)
        return;

    // Critical section for ordered output
    #pragma omp critical
    {
        cout << root->data << " ";
    }

    // Parallel sections for left and right subtree
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            parallelDFS(root->left);
        }

        #pragma omp section
        {
            parallelDFS(root->right);
        }
    }
}

int main() {

    Node* root = new Node(1);

    root->left = new Node(2);
    root->right = new Node(3);

    root->left->left = new Node(4);
    root->left->right = new Node(5);

    root->right->left = new Node(6);
    root->right->right = new Node(7);

    cout << "Parallel DFS Traversal: ";

    parallelDFS(root);

    cout << endl;

    return 0;
}