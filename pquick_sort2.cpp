#ifndef pquick_sort2_cpp
#define pquick_sort2_cpp

#include <iterator>
#include <functional>
#include <algorithm>
#include "med3.h"
#include "insertion_sort.cpp"

template <class RAI, class C, class D>
const typename std::iterator_traits<RAI>::value_type
pq_pseudo_median(D n, RAI a, C cmp, D d)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        if (n <= d) return a[n/2];
        const dif_t m = n / 3;
        const val_t x = pq_pseudo_median(m,       a,         cmp, d);
        const val_t y = pq_pseudo_median(n - 2*m, a + m,     cmp, d);
        const val_t z = pq_pseudo_median(m,       a + n - m, cmp, d);
        return med3(x, y, z, cmp);
}

template <class RAI, class C, class D>
void pquick_sort2(RAI a0, RAI aN, C cmp, D d)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        const dif_t N = aN - a0;
        if (N < 40) { insertion_sort(a0, aN, cmp); return; }
        const val_t pivot = pq_pseudo_median(N, a0, cmp, d);
        RAI ai = a0, aj = aN - 1;
        for (;;) {
                while (cmp(*ai, pivot)) ++ai;
                while (cmp(pivot, *aj)) --aj;
                if (ai >= aj) break;
                std::swap(*ai, *aj);
                ++ai; --aj;
        }
        ++aj;
        if (ai - a0 > 1) pquick_sort2(a0, ai, cmp, d);
        if (aN - aj > 1) pquick_sort2(aj, aN, cmp, d);
}

template <class RAI, class C>
void pquick_sort2(RAI a0, RAI aN, C cmp)
{
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        dif_t N = aN - a0;
        if (N < 2) return;
        dif_t depth;
        for (depth = 0; N > 0; N /= 2) depth++;
        pquick_sort2(a0, aN, cmp, 2*depth);
}

template <class RAI>
inline void pquick_sort2(RAI a0, RAI aN)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        pquick_sort2(a0, aN, std::less<val_t>());
}

#endif