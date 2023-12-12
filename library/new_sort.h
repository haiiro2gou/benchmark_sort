#ifndef new_sort_h
#define new_sort_h

#include <iterator>
#include <functional>
#include <algorithm>

#include "comb_sort.h"

template <class RAI, class T, class C>
void idown_heap(RAI a, T p, T q, C cmp)
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
void imake_heap(RAI a, const T N, C cmp)
{
    for (T p = N/2; p > 0;)
        idown_heap(a, --p, N-1, cmp);
}

template <class RAI, class T, class C>
void isort_heap(RAI a, const T N, C cmp)
{
    for (T q = N-1; q > 0;) {
        std::swap(a[0], a[q]);
        if (--q < 32) { comb_sort(a, a+q+1, cmp); return; }
        idown_heap(a, T(0), q, cmp);
    }
}

template <class RAI, class C>
void new_sort(RAI a0, RAI aN, C cmp)
{
    typedef typename std::iterator_traits<RAI>::value_type val_t;
    typedef typename std::iterator_traits<RAI>::difference_type dif_t;
    const dif_t N = aN - a0;
    if (N < 2) return;

    const double n_dig = log10(N);
    const double s_dig = log10((*a0).data.length());

    if (n_dig - s_dig > -1) {
        imake_heap(a0, N, cmp);
        isort_heap(a0, N, cmp);
    }
    else {
        comb_sort(a0, aN, cmp);
    }
}

template <class RAI>
inline void new_sort(RAI a0, RAI aN)
{
    typedef typename std::iterator_traits<RAI>::value_tyep val_t;
    new_sort(a0, aN, std::less<val_t>());
}

#endif // new_sort_h