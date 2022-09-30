//
// Created by marhl on 9/29/2022.
//

#ifndef TREE_HUGGING_BTREE_H
#define TREE_HUGGING_BTREE_H

#include <cstdio>
#include <vector>
#include <array>
#include <memory>
#include <iostream>
#include <functional>

// Implementation is heavily based on https://www.geeksforgeeks.org/insert-operation-in-b-tree/ (Just modernized a bit)

using namespace std;

template<typename... KeysT> class node_data{ // typename ValuesT,

    tuple<KeysT ...> keys;
    //const ValuesT value;
public:
    node_data(tuple<KeysT ...> & keys);

    bool operator==(node_data& rhs);
    bool operator>(const node_data & rhs);
};

template<typename... KeysT>
bool node_data<KeysT...>::operator==(node_data &rhs){
    return std::apply([&](KeysT const&... values){
        return ((values==rhs.keys)&&...);
    }, keys);
}

template<typename... KeysT>
bool node_data<KeysT...>::operator>(const node_data &rhs) {
    return std::apply([&](KeysT const&... values){
        return ((values>rhs.keys)&&...);
    }, keys);
}

template<typename... KeysT>
node_data<KeysT...>::node_data(tuple<KeysT...> &keys) {
    this->keys = keys;
}

template<int t, typename ... T> class BTree;
template<int t, typename ... T> struct btree_node {
    array<shared_ptr<btree_node>, 2*t> children;
    array<node_data<T ...>, 2*t-1> keys;
    int n{};
    bool leaf;

    friend class BTree<t, T ...>;
private:
    void insertWhenNotFull(node_data<T ...>& key);
    void splitChild(int i, shared_ptr<btree_node<t, T ...>> y);
public:
    void traverse(std::function<void(node_data<T ...>&)> traveller);
    explicit btree_node(bool leaf);
};

template<int m, typename... T>
btree_node<m, T...>::btree_node(bool _leaf) {
   leaf=_leaf;
}

template<int t, typename ... T> class BTree {

    shared_ptr<btree_node<t, T ...>> root{};

private:
public:
    BTree();
    void traverse(std::function<void(node_data<T ...>&)> traveller);
    void insert(node_data<T ...>& key);
};


#endif //TREE_HUGGING_BTREE_H
