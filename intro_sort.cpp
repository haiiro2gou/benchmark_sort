#ifndef intro_sort_cpp
#define intro_sort_cpp

#include <iterator>
#include <functional>
#include <algorithm>
#include "med3.h"
#include "insertion_sort.cpp"
#include "heap_sort1.cpp"

template <class RAI, class C>
static void intro_sort(RAI a0, RAI aN, C cmp, int d);

template <class RAI, class C>
void intro_sort(RAI a0, RAI aN, C cmp) // この関数から呼び出す。
{
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        dif_t N = aN - a0;
        int d;
        for (d = 0; N > 1; N >>= 1) d++;
        intro_sort(a0, aN, cmp, 2 * d);
}

template <class RAI>
inline void intro_sort(RAI a0, RAI aN)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        intro_sort(a0, aN, std::less<val_t>());
}

template <class RAI, class C>
static void intro_sort(RAI a0, RAI aN, C cmp, int d)
{
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        const dif_t N = aN - a0;
        const dif_t M = N - 1;
        if (N < 40) { insertion_sort(a0, aN, cmp); return; }
        if (d == 0) { heap_sort1(a0, aN, cmp); return; }
        const RAI& a = a0;
        const val_t p = med3(a[0], a[N >> 1], a[M], cmp);
        RAI ai = a0, aj = a0 + M;
        for (;;) {
                while (cmp(*ai, p)) ++ai;
                while (cmp(p, *aj)) --aj;
                if (ai >= aj) break;
                std::swap(*ai, *aj);
                ++ai; --aj;
        }
        ++aj;
        const dif_t d2 = d - 1;
        if (ai - a0 > 1) intro_sort(a0, ai, cmp, d2);
        if (aN - aj > 1) intro_sort(aj, aN, cmp, d2);
}

#endif // intro_sort_cpp