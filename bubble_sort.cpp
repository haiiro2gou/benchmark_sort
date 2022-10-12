#ifndef bubble_sort_cpp
#define bubble_sort_cpp

#include <algorithm>
#include <iterator>
#include <functional>

template <class FI, class C>
void bubble_sort(FI a0, FI aN, C cmp)
{
        if (a0 == aN) return;
        FI i, j, k = aN;
        bool done;
        do {
                done = true;
                for (i = j = a0, ++j; j != k; ++i, ++j) {
                        if (cmp(*j, *i)) {
                                std::swap(*i, *j);
                                done = false;
                        }
                }
                k = i;
        } while (!done);
}

template <class FI>
inline void bubble_sort(FI a0, FI aN)
{
        typedef typename std::iterator_traits<FI>::value_type val_t;
        bubble_sort(a0, aN, std::less<val_t>());
}

#endif