#ifndef comb_sort_cpp
#define comb_sort_cpp

#include <algorithm>
#include <iterator>
#include <functional>

template <class FI, class C>
void comb_sort(FI a0, FI aN, C cmp)
{
        typedef typename std::iterator_traits<FI>::difference_type dif_t;
        const dif_t N = std::distance(a0, aN);
        if (N < 2) return;
        dif_t gap = N;
        bool done = false;
        while (!done || gap > 1) {
                gap = dif_t(gap / 1.3);
                if (gap == 9 || gap == 10) gap = 11;
                else if (gap == 0) gap = 1;
                done = true;
                FI i, j; i = j = a0;
                std::advance(j, gap);
                while (j != aN) {
                        if (cmp(*j, *i)) { std::swap(*i, *j); done = false; }
                        ++i, ++j;
                }
        }
}

template <class FI>
inline void comb_sort(FI a0, FI aN)
{
        typedef typename std::iterator_traits<FI>::value_type val_t;
        comb_sort(a0, aN, std::less<val_t>());
}

#endif