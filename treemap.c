#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2))
{   
    TreeMap* map = (TreeMap*)malloc(sizeof(TreeMap));
    if (map == NULL) 
    {
        return NULL;
    }
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;

    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value)
{
    if (searchTreeMap(tree, key) != NULL)
    {
        return;
    }

    TreeNode* newNode = createTreeNode(key, value);
    TreeNode* parent = NULL;
    TreeNode* current = tree->root;

    while (current != NULL)
    {
        parent = current;

        if (tree->lower_than(key, current->pair->key))
        {
            current = current->left;
        }

        else
        {
            current = current->right;
        }
    }

        newNode->parent = parent;

        if (parent == NULL)
        {
            tree->root = newNode;
        }
        else if (tree->lower_than(key, parent->pair->key))
        {
            parent->left = newNode;
        }
        else
        {
            parent->right = newNode;
        }
    
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x)
{
    if (x == NULL)
    {
        return NULL;
    }

    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node)
{
    if (node->right == NULL && node->left == NULL)
    {
        if (node->parent == NULL)
        {
            tree->root = NULL;
        }
    else
    {
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        else
        {
            node->parent->right = NULL;
        }
    }
        free(node->pair);
        free(node);
        return;
    }

    if (node->left == NULL || node->right == NULL)
    {
        TreeNode* hijo = (node->left != NULL) ? node->left : node->right;

        if (node->parent == NULL)
        {
            tree->root = hijo;
        }
        else
        {
            if (node->parent->left == node)
            {
                node->parent->left = hijo;
            }
            else
            {
                node->parent->right = hijo;
            }
        }

        hijo->parent = node->parent;

        free(node->pair);
        free(node);
        return;
    } 

    TreeNode* sucesor = minimum(node->right);
    node->pair = sucesor->pair;
    removeNode(tree, sucesor);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key)
{
    TreeNode* current = tree->root;
    
    while (current != NULL)
    {
        if (is_equal(tree, key, current->pair->key))
    {
        tree->current = current;
        return current->pair;
    }

    if (tree->lower_than(key, current->pair->key))
    {
        current = current->left;
    }
    else
    {
        current = current->right;
    }
}
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
    TreeNode* current = tree->root;
    TreeNode* node_upper = NULL;

    while (current != NULL)
    {
        if (is_equal(tree, key, current->pair->key))
        {
            tree->current = current;
            return current->pair;
        }

        if (tree->lower_than(key, current->pair->key))
        {
            node_upper = current;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    if (node_upper != NULL)
    {
        tree->current = node_upper;
        return node_upper->pair;
    }
    
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree)
{
    if (tree->root == NULL)
    {
        return NULL;
    }

    TreeNode* minimo = minimum(tree->root);
    tree->current = minimo;
    return minimo->pair;
}

Pair * nextTreeMap(TreeMap * tree)
{
    if (tree->current == NULL)
    {
        return NULL;
    }

    TreeNode* current = tree->current;

    if (current->right != NULL)
    {
        current = minimum(current->right);
        tree->current = current;
        return current->pair;
    }

    TreeNode* parent = current->parent;
    while (parent != NULL && parent->right == current)
    {
        current = parent;
        parent = parent->parent;
    }

    tree->current = parent;
    if (parent == NULL)
    {
        return NULL;
    }
    return parent->pair;
}
