//
// Created by marhl on 9/29/2022.
//

#include "btree.h"


template<int t, typename ... T> void BTree<t, T ...>::insert(node_data<T ...>& key)
{
    // If tree is empty
    if (root == nullptr)
    {
        // Allocate memory for root
        root = shared_ptr<btree_node<t, T...>>(new btree_node<t, T...>(true));
        root->keys[0] = key;
        root->n = 1;  // Update number of values in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2*t - 1)
        {
            // Allocate memory for new root
            auto s = shared_ptr<btree_node<t, T...>>(new btree_node<t, T...>(false));

            // Make old root as child of new root
            s->C[0] = root;

            // Split the old root and move 1 key to the new root
            s->splitChild(0, root);

            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->keys[0] < key)
                i++;
            s->C[i]->insertNonFull(key);

            // Change root
            root = s;
        }
        else  // In case the root note still has space
        {
            root->insertWhenNotFull(key);
        }
    }
}


template<int t, typename ... T> void btree_node<t, T...>::insertWhenNotFull(node_data<T ...>& key)
{
    // Initialize index as index of rightmost element
    int i = n-1;

    // If this is a leaf node
    if (leaf)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && keys[i] > key)
        {
            keys[i+1] = keys[i];
            i--;
        }

        // Insert the new key at found location
        keys[i+1] = key;
        n = n+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > key)
            i--;

        // See if the found child is full
        if (children[i+1]->n == 2*t-1)
        {
            // If the child is full, then split it
            splitChild(i+1, children[i+1]);

            // After split, the middle key of C[i] goes up and
            // C[i] is split into two. See which of the two
            // is going to have the new key
            if (keys[i+1] < key)
                i++;
        }
        children[i+1]->insertNonFull(key);
    }
}

template<int t, typename ... T> void btree_node<t, T ...>::splitChild(int i, shared_ptr<btree_node<t, T ...>> y)
{
    // Create a new node which is going to store (t-1) keys
    // of y
    auto z = shared_ptr<btree_node<t, T ...>>(new btree_node(y->t, y->leaf));
    z->n = t - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];

    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }

    // Reduce the number of keys in y
    y->n = t - 1;

    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        children[j+1] = children[j];

    // Link the new child to this node
    children[i+1] = z;

    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];

    // Copy the middle key of y to this node
    keys[i] = y->keys[t-1];

    // Increment count of keys in this node
    n = n + 1;
}

template<int t, typename... T>
void BTree<t, T...>::traverse(std::function<void(node_data<T...> &)> traveller) {
    root->traverse(traveller);
}

template<int t, typename... T>
BTree<t, T...>::BTree() = default;

template<int t, typename ... T> void btree_node<t,T...>::traverse(std::function<void(node_data<T ...>&)> traveller)
{
    // There are n keys and n+1 children, traverse through n
    // keys and first n children
    int i;
    for (i = 0; i < n; i++) {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (!leaf)
            children[i]->traverse();

        traveller(keys[i]);
        //cout << " " << ;
    }

    // Print the subtree rooted with last child
    if (!leaf)
        children[i]->traverse();
}
