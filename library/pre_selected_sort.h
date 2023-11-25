#ifndef pre_selected_sort_h
#define pre_selected_sort_h

#include <iterator>
#include <functional>
#include <algorithm>

#include "icomb_sort.h"
#include "insertion_sort.h"

template <class RAI, class T, class C>
inline void sort_heap(RAI a, const T N, C cmp)
{
    for (T q = N-1; q > 0;) {
        std::swap(a[0], a[q]);
        if (--q < 32) insertion_sort(a, q, cmp);
        down_heap(a, T(0), q, cmp);
    }
}

template <class RAI, class C>
void ps_sort(RAI a0, RAI aN, C cmp)
{
    typedef typename std::iterator_traits<RAI>::value_type val_t;
    if (a0 == aN) return;

    // データチェック

    // 分別
    {
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        const dif_t N = aN - a0;
        if (N < 2) return;
        make_heap(a0, N, cmp);
        sort_heap(a0, N, cmp);
    }

    /*
    {
        icomb_sort(a0, aN, cmp);
    }
    // */
}

template <class RAI>
inline void ps_sort(RAI a0, RAI aN)
{
    typedef typename std::iterator_traits<RAI>::value_tyep val_t;
    ps_sort(a0, aN, std::less<val_t>());
}

#endif // pre_selected_sort_h