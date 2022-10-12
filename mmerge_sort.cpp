#ifndef mmerge_sort_cpp
#define mmerge_sort_cpp

#include <iterator>
#include <vector>
#include <functional>
#include "insertion_sort.cpp"

template <class BI1, class BI2, class C, class D>
static void mmerge_sort(BI1 a0, BI1 aN, BI2 b0, BI2 bN, C cmp, D N);

template <class BI, class C>
inline void mmerge_sort(BI a0, BI aN, C cmp)
{
        typedef typename std::iterator_traits<BI>::difference_type dif_t;
        typedef typename std::iterator_traits<BI>::value_type val_t;
        std::vector<val_t> b(a0, aN);
        const dif_t N = b.size();
        mmerge_sort(b.begin(), b.end(), a0, aN, cmp, N);
}

template <class BI>
inline void mmerge_sort(BI a0, BI aN)
{
        typedef typename std::iterator_traits<BI>::value_type val_t;
        mmerge_sort(a0, aN, std::less<val_t>());
}

template <class BI1, class BI2, class C, class D>
static void mmerge_sort(BI1 a0, BI1 aN, BI2 b0, BI2 bN, C cmp, D N)
{
        if (N < 34) { insertion_sort(b0, bN, cmp); return; }
        D M = N >> 1;
        BI1 aM = a0; std::advance(aM, M);
        BI2 bM = b0; std::advance(bM, M);
        mmerge_sort(b0, bM, a0, aM, cmp, M);
        mmerge_sort(bM, bN, aM, aN, cmp, N - M);
        BI1 ai = a0, aj = aM;
        BI2 bk = b0;
        while (ai != aM && aj != aN) {
                if (cmp(*aj, *ai))
                        *bk = *aj, ++bk, ++aj;
                else
                        *bk = *ai, ++bk, ++ai;
        }
        std::copy(ai, aM, bk);
        std::copy(aj, aN, bk);
}

#endif