#ifndef quick_sort_h
#define quick_sort_h

#include <algorithm>
#include <iterator>
#include <functional>

template <class RAI, class C>
void quick_sort(RAI a0, RAI aN, C cmp)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        if (a0 == aN) return;
        RAI ai = a0, aj = aN - 1;
        const val_t pivot = *a0;
        for (;;) {
                while (cmp(*ai, pivot)) ++ai;
                while (cmp(pivot, *aj)) --aj;
                if (ai >= aj) break;
                std::swap(*ai, *aj);
                ++ai; --aj;
        }
        ++aj;
        if (ai - a0 > 1) quick_sort(a0, ai, cmp);
        if (aN - aj > 1) quick_sort(aj, aN, cmp);
}

template <class RAI>
inline void quick_sort(RAI a0, RAI aN)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        quick_sort(a0, aN, std::less<val_t>());
}

#endif