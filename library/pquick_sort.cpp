#ifndef pquick_sort_cpp
#define pquick_sort_cpp

#include <algorithm>
#include <iterator>
#include <functional>
#include "med3.h"
#include "insertion_sort.cpp"

template <class RAI, class C>
void pquick_sort(RAI a0, RAI aN, C cmp)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        const dif_t N = aN - a0;
        const dif_t M = N - 1;
        if (N < 40) { insertion_sort(a0, aN, cmp); return; }
        const RAI& a = a0;
        const val_t pivot = med3(a[0], a[N >> 1], a[M], cmp);
        RAI ai = a0, aj = a0 + M;
        for (;;) {
                while (cmp(*ai, pivot)) ++ai;
                while (cmp(pivot, *aj)) --aj;
                if (ai >= aj) break;
                std::swap(*ai, *aj);
                ++ai; --aj;
        }
        ++aj;
        if (ai - a0 > 1) pquick_sort(a0, ai, cmp);
        if (aN - aj > 1) pquick_sort(aj, aN, cmp);
}

template <class RAI>
inline void pquick_sort(RAI a0, RAI aN)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        pquick_sort(a0, aN, std::less<val_t>());
}

#endif