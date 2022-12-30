#ifndef merge_sort2_cpp
#define merge_sort2_cpp

#include <iterator>
#include <vector>
#include <functional>
#include <algorithm>

template <class FI, class B, class D, class C>
FI merge_sort2(FI a0, B b0, D N, C cmp);

template <class FI, class C>
inline void merge_sort2(FI a0, FI aN, C cmp)
{
        typedef typename std::iterator_traits<FI>::value_type val_t;
        typedef typename std::iterator_traits<FI>::difference_type dif_t;
        const dif_t N = std::distance(a0, aN);
        const dif_t M = N / 2;
        std::vector<val_t> b(M);
        merge_sort2(a0, b.begin(), N, cmp);
}

template <class FI>
inline void merge_sort2(FI a0, FI aN)
{
        typedef typename std::iterator_traits<FI>::value_type val_t;
        merge_sort2(a0, aN, std::less<val_t>());
}

template <class FI, class B, class C>
void merge(FI a0, FI aM, FI aN, B b0, C cmp)
{
        B bi = b0, bM = std::copy(a0, aM, b0);
        FI aj = aM, ak = a0;
        while (bi != bM && aj != aN) {
                if (cmp(*aj, *bi))
                        *ak = *aj, ++ak, ++aj;
                else
                        *ak = *bi, ++ak, ++bi;
        }
        std::copy(bi, bM, ak);
}

template <class FI, class B, class D, class C>
FI merge_sort2(FI a0, B b0, D N, C cmp)
{
        if (N == 1) return ++a0;
        D M = N / 2;
        FI aM = merge_sort2(a0, b0, M,     cmp);
        FI aN = merge_sort2(aM, b0, N - M, cmp);
        merge(a0, aM, aN, b0, cmp);
        return aN;
}

#endif