#ifndef heap_sort1_cpp
#define heap_sort1_cpp

#include <iterator>
#include <functional>
#include <algorithm>

template <class RAI, class T, class C>
void down_heap(RAI a, T p, T q, C cmp)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        T c;
        const val_t tmp = a[p];
        while ((c = 2*p + 1) <= q) {
                if (c < q && cmp(a[c], a[c+1])) c++;
                if (!cmp(tmp, a[c])) break;
                a[p] = a[c]; p = c;
        }
        a[p] = tmp;
}

template <class RAI, class T, class C>
void make_heap(RAI a, const T N, C cmp)
{
        for (T p = N/2; p > 0;)
                down_heap(a, --p, N-1, cmp);
}

template <class RAI, class T, class C>
void sort_heap(RAI a, const T N, C cmp)
{
        for (T q = N-1; q > 0;) {
                std::swap(a[0], a[q]);
                down_heap(a, T(0), --q, cmp);
        }
}

template <class RAI, class C>
void heap_sort1(RAI a0, RAI aN, C cmp)
{
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        const dif_t N = aN - a0;
        if (N < 2) return;
        make_heap(a0, N, cmp);
        sort_heap(a0, N, cmp);
}

template <class RAI>
inline void heap_sort1(RAI a0, RAI aN)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        heap_sort1(a0, aN, std::less<val_t>());
}

#endif