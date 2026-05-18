#include <iostream>
#include <vector>
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

// Parallel BFS Function
void parallelBFS(Node* root) {

    if (root == NULL)
        return;

    vector<Node*> currentLevel;
    currentLevel.push_back(root);

    cout << "Parallel BFS Traversal: ";

    while (!currentLevel.empty()) {

        vector<Node*> nextLevel;

        // Print current level
        for (Node* node : currentLevel) {
            cout << node->data << " ";
        }

        // Parallel processing of current level
        #pragma omp parallel
        {
            vector<Node*> localNext;

            #pragma omp for nowait
            for (int i = 0; i < currentLevel.size(); i++) {

                Node* node = currentLevel[i];

                if (node->left)
                    localNext.push_back(node->left);

                if (node->right)
                    localNext.push_back(node->right);
            }

            // Merge local results safely
            #pragma omp critical
            {
                nextLevel.insert(nextLevel.end(),
                                 localNext.begin(),
                                 localNext.end());
            }
        }

        currentLevel = nextLevel;
    }

    cout << endl;
}

int main() {

    Node* root = new Node(1);

    root->left = new Node(2);
    root->right = new Node(3);

    root->left->left = new Node(4);
    root->left->right = new Node(5);

    root->right->left = new Node(6);
    root->right->right = new Node(7);

    parallelBFS(root);

    return 0;
}