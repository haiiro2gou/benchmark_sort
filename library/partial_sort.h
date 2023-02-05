#ifndef partial_sort_h
#define partial_sort_h

#include <iterator>
#include <functional>
#include <algorithm>
#include "heap_sort2.h"

template <class RAI, class C>
void my_partial_sort(RAI a0, RAI aM, RAI aN, C cmp)
{
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        const dif_t M = aM - a0, M1 = M - 1;

        if (M == 0) return;
        for (dif_t c = 1; c < M; c++) up_heap(a0, c, cmp);
        for (RAI p = aM; p < aN; ++p) {
                if (cmp(*p, *a0)) {
                        std::swap(*a0, *p);
                        down_heap(a0, 0, M1, cmp);
                }
        }
        for (dif_t q = M1; q > 0; q--) {
                std::swap(a0[0], a0[q]);
                down_heap(a0, 0, q - 1, cmp);
        }
}

template <class RAI>
inline void my_partial_sort(RAI a0, RAI aM, RAI aN)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        my_partial_sort(a0, aM, aN, std::less<val_t>());
}

#endif