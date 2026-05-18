#include<iostream>
#include<omp.h>
#include<queue>
using namespace std;

class Node{

    public:
    int data;
    Node* left = NULL;
    Node* right = NULL;
    Node(int data){
        this->data = data;
        this->left = left;
        this->right= right;
    }

};

Node* connectNodes(Node* nodes[] , int n){
    for(int i=0;i<n;i++){
        int leftIndx = 2*i+1;
        int rightIndx = 2*i+2;
        if(leftIndx<n){
            nodes[i]->left = nodes[leftIndx];
        }
        if(rightIndx<n){
            nodes[i]->right = nodes[rightIndx];
        }
    }
    return nodes[0];
}

void dfs(Node* root){
    if(root==NULL) return;

    #pragma omp critical
    cout<<root->data<<" ->";
    #pragma omp parallel sections
    {
        #pragma omp section
        {

            dfs(root->left);
        }
        #pragma omp section
        {

            dfs(root->right);
        }
    }
}

void bfs(Node* root){
    queue<Node*> q;
    q.push(root);
    while(!q.empty()){  
        int size = q.size();
        
        #pragma omp parallel for
        for(int i=0;i<size;i++){
            Node* current;
            #pragma omp critical
            {
                current = q.front();
                q.pop();
                cout<<current->data<<" ->";
                if(current->left!=NULL){
                    q.push(current->left);
                }
                if(current->right!=NULL){
                    q.push(current->right);
                }
            }
        }
    }
}

int main(){

    int n = 10;
    Node* nodes[n];
    for(int i=0;i<n;i++){
        nodes[i] = new Node(i);
    }

    Node* root = connectNodes(nodes,n);


    
    cout<<"After DFS --> ";
    double startdfs = omp_get_wtime();
    dfs(root);
    double enddfs = omp_get_wtime();
    cout<<"\n DFS Execution Time : "<<(enddfs-startdfs)*1000<<" Miliseconds";
    
    cout<<"\nAfter BFS --> ";
    double startbfs = omp_get_wtime();
    bfs(root);
    double endbfs = omp_get_wtime();
    cout<<"\n BFS Execution Time : "<<(endbfs-startbfs)*1000<<" Miliseconds";
      
    return 0;
}