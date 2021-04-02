#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node {
    bool color;
    uint8_t key;
    struct node *parent;
    struct node *left;
    struct node *right;
} Node;

typedef struct tree {
    Node *nil;
    Node *root;
} Tree;

Tree *create_tree() {
    Tree *new_tree = (Tree *)malloc(sizeof(Tree));
    Node *nil = (Node *)malloc(sizeof(Node));
    new_tree->nil = nil;

    return new_tree;
}

Node *create_node(uint8_t key) {
    Node *new_node = (Node *)malloc(sizeof(Node));

    new_node->key = key;
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

void left_rotate(Tree *tree, Node *x) {
    Node *y = x->right;
    x->right = y->left;

    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!(x->parent)) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

void right_rotate(Tree *tree, Node *x) {
    Node *y = x->left;
    x->left = y->right;

    if(y->right) y->right->parent = x;
    y->parent = x->parent;
    if(!(x->parent)) tree->root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;

    y->right = x;
    x->parent = y;
}

void rb_insert_fixup(Tree *tree, Node *x) {
    Node *y = (Node *)malloc(sizeof(Node));
    while (x->parent->color) {
        if (x->parent == x->parent->parent->left) {
            y = x->parent->parent->right;

            if (!(x->color)) {
                x->parent->color = false;
                y->color = false;
                x->parent->parent->color = true;
                x = x->parent->parent;
            }
            else if (x == x->parent->right) {
                x = x->parent;
                left_rotate(tree, x);
            }
            x->parent->color = false;
            x->parent->parent->color = true;
            right_rotate(tree, x->parent->parent);
        }
    }
    tree->root->color = false;
}

void insert_node(Tree *tree, Node *new_node) {
    Node **current = malloc(sizeof(Node));
    Node **previous = malloc(sizeof(Node));
    
    *previous = tree->nil;
    *current = tree->root;
    
    while ((*current)) {
        *previous = *current;

        if ((*current)->key > new_node->key) *current = (*current)->left;
        else *current = (*current)->right;
    }

    new_node->parent = (*previous);
    
    if ((*previous) == tree->nil) {
        tree->root = new_node;
        tree->root->parent = tree->nil;
        return;
    }

    else if ((*previous)->key < new_node->key) (*previous)->right = new_node;
    else (*previous)->left = new_node;

    new_node->left = tree->nil;
    new_node->right = tree->nil;
    new_node->color = true;

    rb_insert_fixup(tree, new_node);

    free(current);
    free(previous);  
}

Node *tree_minimum(Node *x) {
    while(x->left) x = x->left;
    return x;
}

Node *tree_maximum(Node *x) {
    while(x->right) x = x->right;
    
    return x;
}

Node *tree_sucessor(Node *x) {
    Node *y = x->parent;
    
    if (x->right) return tree_minimum(x->right);
    
    while(y && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

Node *tree_predecessor(Node *x) {
    Node *y = x->parent;
    
    if (x->left) return tree_maximum(x->left);
    
    while(y && x == y->left) {
        x = y;
        y = y->parent;
    }
    return y;
}

void transplant(Tree *tree, Node *u, Node *v) {
    if (!(u->parent)) {
        tree->root = v;
        return;
    }
    else if (u == u->parent->left) u->parent->left = v;
    
    else u->parent->right = v;
    
    if (v) v->parent = u->parent;
}

void delete_node(Tree *tree, Node *x) {
    Node *y = (Node *)malloc(sizeof(Node));

    if (!(x->left)) { 
        transplant(tree, x, x->right);
        return;
    }
    else if (!(x->right)) { 
        transplant(tree, x, x->left);
        return;
    }
    else {
        y = tree_minimum(x->right);
        
        if (y != x->parent) {
            transplant(tree, y, y->right);
            y->right = x->right;
            y->right->parent = y;
        }
        transplant(tree, x, y);
        y->left = x->left;
        y->left->parent = y;
    }  
}


Node *binary_search(Tree *tree, uint8_t to_find) {
    Node *current = tree->root;

    while (current && current->key != to_find) {
        if (current->key < to_find) current = current->right;
        else current = current->left;
    }

    return current;
}

void inorder_walk(Tree *tree, Node *x) {
    if (!x) return;
    inorder_walk(x->left);
    printf("%d ", x->key);
    inorder_walk(x->right);
}


void main() {
    Tree *t = create_tree(); 
    Node *current = (Node *)malloc(sizeof(Node));
    Node *to_insert = create_node(10);
    
    insert_node(t, to_insert);
    to_insert = create_node(3);
    
    insert_node(t, to_insert);
    /*
    to_insert = create_node(12);
    insert_node(t, to_insert);
    to_insert = create_node(13);
    insert_node(t, to_insert);
    to_insert = create_node(6);
    insert_node(t, to_insert);
    to_insert = create_node(5);
    insert_node(t, to_insert);
    current = binary_search(t, 6);
    delete_node(t, current);
    //current = tree_minimum(current);
    //printf("%d\n", current->key);
    */
    inorder_walk(t->root);
    //current = binary_search(t, 5);
    //current = tree_sucessor(current); 
    //current = tree_predecessor(current);
    //printf("%d\n", current->key);
    free(current);
}
