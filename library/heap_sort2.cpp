#ifndef heap_sort2_cpp
#define heap_sort2_cpp

#include <iterator>
#include <functional>
#include <algorithm>
#include "heap_sort1.cpp"

template <class RAI, class T, class C>
inline void up_heap(RAI a, T c, C cmp)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        const val_t tmp = a[c];
        while (c > 0) {
                const T p = (c - 1) >> 1;
                if (!cmp(a[p], tmp)) break;
                a[c] = a[p]; c = p;
        }
        a[c] = tmp;
}

template <class RAI, class C>
void heap_sort2(RAI a0, RAI aN, C cmp)
{
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        const dif_t N = aN - a0;
        if (N < 2) return;
        for (dif_t c = 1; c < N; c++) up_heap(a0, c, cmp);
        sort_heap(a0, N, cmp);
}

template <class RAI>
inline void heap_sort2(RAI a0, RAI aN)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        heap_sort2(a0, aN, std::less<val_t>());
}

#endif