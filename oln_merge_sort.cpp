#ifndef oln_merge_sort_cpp
#define oln_merge_sort_cpp

#include <list>
#include <functional>
#include "insertion_sort.cpp"

template <class T, class C, class D>
void oln_merge_sort(std::list<T>& a, C cmp, D N);

template <class T, class C>
inline void oln_merge_sort(std::list<T>& a, C cmp)
{
        oln_merge_sort(a, cmp, a.size());
}

template <class T>
inline void oln_merge_sort(std::list<T>& a)
{
        oln_merge_sort(a, std::less<T>());
}

template <class T, class C, class D>
void oln_merge_sort(std::list<T>& a, C cmp, D N)
{
        if (N < 36) { insertion_sort(a.begin(), a.end(), cmp); return; }
        D M = N >> 1;
        typename std::list<T>::iterator aM = a.begin(); std::advance(aM, M);
        std::list<T> b;
        b.splice(b.end(), a, aM, a.end());
        oln_merge_sort(a, cmp, M);
        oln_merge_sort(b, cmp, N - M);
        a.merge(b, cmp);
}

#endif