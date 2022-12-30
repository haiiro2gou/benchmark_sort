#ifndef insertion_sort2_cpp
#define insertion_sort2_cpp

#include <iterator>
#include <functional>

template <class BI, class C>
void insertion_sort2(BI a0, BI aN, C cmp)
{
        typedef typename std::iterator_traits<BI>::value_type val_t;
        if (a0 == aN) return;
        BI i = a0;
        for (++i; i != aN; ++i) {
                const val_t tmp = *i;
                BI j, k; j = k = i;
                while (a0 != j && cmp(tmp, *(--k))) { *j = *k; j = k; }
                *j = tmp;
        }
}

template <class BI>
inline void insertion_sort2(BI a0, BI aN)
{
        typedef typename std::iterator_traits<BI>::value_type val_t;
        insertion_sort2(a0, aN, std::less<val_t>());
}

#endif