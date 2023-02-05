#ifndef selection_sort_h
#define selection_sort_h

#include <algorithm>
#include <iterator>
#include <functional>

template <class FI, class C>
void selection_sort(FI a0, FI aN, C cmp)
{
        for (FI i = a0; i != aN; ++i) {
                FI min_j, j; min_j = j = i;
                for (++j; j != aN; ++j)
                        if (cmp(*j, *min_j)) min_j = j;
                std::swap(*i, *min_j);
        }
}

template <class FI>
inline void selection_sort(FI a0, FI aN)
{
        typedef typename std::iterator_traits<FI>::value_type val_t;
        selection_sort(a0, aN, std::less<val_t>());
}

#endif