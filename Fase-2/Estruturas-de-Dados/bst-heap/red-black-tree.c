#include <stdio.h>
#include <stdlib.h>

struct node {
     int key;
     char c;
     struct node * left;
     struct node * right;
     struct node * parent;
};

typedef struct node Node;

//the following 4 functions was just coded for debugging
int max(int a, int b){
    return (a > b ? a : b);
}

int height(Node * parent){
    if (parent == NULL)
        return 0;
    int left = height(parent->left) + 1;
    int right = height(parent->right) +1;
    return max(left, right);
}

void BFS(Node * parent, int height){
    if (parent == NULL)
        return;
    if (height == 0)
        printf("%d:%c \t", parent->key, parent->c);
    else{
        BFS(parent->left, height-1);
        BFS(parent->right, height-1);
    }
}

void BreadthFirstSearch(Node * parent){
    int h = height(parent);
    for (int i = 0; i < h; i++){
        BFS(parent, i);
        printf("\n");
    }
}

//set node attrs
Node* setNode(Node* parent, int key){
    Node* new = (Node*) malloc(sizeof(Node));
    new->key = key;
    new->left = NULL;
    new->right = NULL;
    new->parent = parent;
    new->c = 'R';
    return new;
}

Node* insert(Node** root, Node* parent, int key){
    if (*root == NULL){
        *root = setNode(parent, key);
        if (parent== NULL){
            (*root)->parent = (Node*) malloc(sizeof(Node));
            (*root)->parent->c = 'B';
            (*root)->parent->left = NULL;
            (*root)->parent->right = NULL;
            (*root)->parent->parent = NULL;
        }
        return *root;
    }
    if (key < (*root)->key)
        return insert(&(*root)->left, *root, key);
    else
        return insert(&(*root)->right, *root, key);
}

Node* leftRotate(Node* new, Node* parent){
    new->c = 'B';
    parent->c = 'R';
    new->parent = parent->parent;
    parent->right = new->left;
    new->left = parent;
    parent->parent->right = new;
    parent->parent = new;

    if (new->left->right != NULL)
        new->left->right->parent = parent;

    return new;
}

Node* rightRotate(Node* new, Node* parent){
    new->c = 'B';
    parent->c = 'R';
    new->parent = parent->parent;
    parent->left = new->right;
    new->right = parent;
    parent->parent->left = new;
    parent->parent = new;

    if (new->right->left != NULL)
        new->right->left->parent = parent;

    return new;
}

Node* fixUpInsertion(Node* root, Node* new){
    //printf("Key: %d Parent: %d\n", new->key, new->parent->key);
    while (new->parent != NULL && new->parent->c == 'R'){
        //printf("%d == %d", new->parent->key, new->parent->parent->left->key);
        Node* grandparent = new->parent->parent;
        if (new->parent == new->parent->parent->left){
            Node* uncle = new->parent->parent->right;

            //null means a black node
            if (uncle != NULL && uncle->c == 'R'){
                printf("Changing colors: grandparent <---> parent and uncle!\n");
                new->parent->c = 'B';
                uncle->c = 'B';
                new->parent->parent->c = 'R';
                //new = new->parent;
            }
            // left-right case
            else if (new->parent->right == new){
                printf("Caso left-right!\n");
                new->parent->parent->left = new;
                new->parent->right = new->left;
                new->left = new->parent;
                new->parent->parent = new;
                new->parent = grandparent;
                printf("%d\n", new->parent->parent->key);
                new = rightRotate(new, new->parent);
            }
            else{
                printf("Caso left-left!\n");
                new = rightRotate(new->parent, new->parent->parent);
                new = new->parent;
                if (new->parent->parent == NULL) root = new;
            }
            //new = new->parent;
        }

        else{
            Node* uncle = new->parent->parent->left;
            if (uncle != NULL && uncle->c == 'R'){
                printf("Changing colors: grandparent <---> parent and uncle!\n");
                new->parent->c = 'B';
                uncle->c = 'B';
                new->parent->parent->c = 'R';
                //new = new->parent;
            }
            else if(new->parent->left == new){
                printf("Caso right-left!\n");
                new->parent->parent->right = new;
                new->parent->left = new->right;
                new->right = new->parent;
                new->parent->parent = new;
                new->parent = grandparent;
                new = leftRotate(new, new->parent);
                //new = leftRotate(new, new->parent);
            }
            else{
                printf("Caso right-right!\n");
                new = leftRotate(new->parent, new->parent->parent);
                if (new->parent->parent == NULL) root = new;
                new = new->parent;
            }
            //when new become the root
        }
    }
    root->c = 'B';
    return root;
}

Node* getNode(Node* root, int key){
    if (root->key == key)
        return root;
    if (key < root->key)
        return getNode(root->left, key);
    else
        return getNode(root->right, key);
}

int main(){
    Node* root = NULL, *new = NULL;

    int value, size;

    scanf("%d", &size);
    for (int i = 0; i < size; i++){
        scanf("%d", &value);
        new = insert(&root , NULL, value);
        //printf("%d\n", new->key);
        root = fixUpInsertion(root, new);
    }
    /*Node* c = getNode(root, 5);
    printf("Debbuging motherfucker!!\n");
    printf("%d\n", root->left->key);
    printf("%d\n", root->left->parent->key);
    printf("Teste do parent: %d\n", root->left->right->parent->key);
    printf("%p\n", root->right->left);*/
    //printf("%d\n", root->right->right->key);
    printf("\t");BreadthFirstSearch(root);
    printf("left-tree height: %d  right-tree height: %d\n", height(root->left), height(root->right));
    return 0;
}
