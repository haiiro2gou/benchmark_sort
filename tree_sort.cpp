#ifndef tree_sort_cpp
#define tree_sort_cpp

#include <list>
#include <algorithm>
#include <iterator>

template <class T> class Tree
{
    struct Node
    {
        std::list<T> bucket;
        Node *lst;
        Node *rst;
        Node(const T& x) : lst(0), rst(0) { bucket.push_back(x); }
    } *root; 

    Tree() : root(0) {}

    template <class C> static Node *insert(Node *t, const T& x, C cmp)
    {
        if (t == 0) return new Node(x);
        const T y = t->bucket.front();
        if (cmp(x, y))
            t->lst = insert(t->lst, x, cmp);
        else if (cmp(y, x))
            t->rst = insert(t->rst, x, cmp);
        else
            t->bucket.push_back(x);
        return t;
    }

    template <class FI> static FI flatten(Node *t, FI a0)
    {
        if (t == 0) return a0;
        a0 = flatten(t->lst, a0);
        a0 = std::copy(t->bucket.begin(), t->bucket.end(), a0);
        a0 = flatten(t->rst, a0);
        delete t;
        return a0;
    }

    template <class FI, class C> void sort(FI a0, FI aN, C cmp)
    {
        for (FI ai = a0; ai != aN; ++ai) root = insert(root, *ai, cmp);
        flatten(root, a0);
    }

    template <class FI, class C> friend void treesort(FI a0, FI aN, C cmp);
};

template <class FI, class C> void treesort(FI a0, FI aN, C cmp)
{
    typedef typename std::iterator_traits<FI>::value_type val_t;
    Tree<val_t>().sort(a0, aN, cmp);
}

template <class FI> void treesort(FI a0, FI aN)
{
    typedef typename std::iterator_traits<FI>::value_type val_t;
    treesort(a0, aN, std::less<val_t>());
}

#endif