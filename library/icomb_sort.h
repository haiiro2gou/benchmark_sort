#ifndef icomb_sort_h
#define icomb_sort_h

#include <iterator>
#include <algorithm>
#include <functional>
#include "insertion_sort.h"

template <class BI, class C>
void icomb_sort(BI a0, BI aN, C cmp)
{
        typedef typename std::iterator_traits<BI>::difference_type dif_t;
        const dif_t N = std::distance(a0, aN);
        if (N < 2) return;
        dif_t gap = N;
        while (gap > 20) {
                gap = dif_t(gap / 1.37);
                if (gap == 9 || gap == 10) gap = 11;
                BI i, j; i = j = a0;
                std::advance(j, gap);
                while (j != aN) {
                        if (cmp(*j, *i)) std::swap(*i, *j);
                        ++i, ++j;
                }
        }
        insertion_sort(a0, aN, cmp);
}

template <class BI>
inline void icomb_sort(BI a0, BI aN)
{
        typedef typename std::iterator_traits<BI>::value_type val_t;
        icomb_sort(a0, aN, std::less<val_t>());
}

#endif