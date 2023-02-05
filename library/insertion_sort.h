#ifndef insertion_sort_h
#define insertion_sort_h

#include <iterator>
#include <functional>

template <class BI, class C>
void insertion_sort(BI a0, BI aN, C cmp)
{
        typedef typename std::iterator_traits<BI>::value_type val_t;
        if (a0 == aN) return;
        BI i = a0;
        for (++i; i != aN; ++i) {
                const val_t tmp = *i;
                BI j, k; j = k = i;
                if (cmp(tmp, *a0))
                        while (a0 != j) { --k; *j = *k; j = k; }
                else 
                        while (cmp(tmp, *(--k))) { *j = *k; j = k; }
                *j = tmp;
        }
}

template <class BI>
inline void insertion_sort(BI a0, BI aN)
{
        typedef typename std::iterator_traits<BI>::value_type val_t;
        insertion_sort(a0, aN, std::less<val_t>());
}

#endif