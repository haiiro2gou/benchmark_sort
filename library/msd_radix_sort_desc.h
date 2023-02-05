#ifndef msd_radix_sort_desc_h
#define msd_radix_sort_desc_h

#include <iterator>
#include <new>
#include "insertion_sort.h"

template <int UNIT, class RAI, class G, class C, class B>
static void msd_radix_sort_desc(int shift,
        RAI a0, RAI aN, G get_key, C cmp, B b);

template <int UNIT, int BITS, class RAI, class G, class C>
void msd_radix_sort_desc(RAI a0, RAI aN, G get_key, C cmp)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        const dif_t N = std::distance(a0, aN);

        val_t* const b = new val_t[N];
        try { msd_radix_sort_desc<UNIT>(BITS - UNIT, a0, aN, get_key, cmp, b); }
        catch (std::bad_alloc&) { delete[] b; throw; }
        delete[] b;
}

template <int UNIT, class RAI, class G, class C, class B>
static void msd_radix_sort_desc(int shift,
        RAI a0, RAI aN, G get_key, C cmp, B b)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        static const int KEYS = 1 << UNIT;
        static const int MASK = KEYS - 1;
        const dif_t N = std::distance(a0, aN);

        if (N < 30) { insertion_sort(a0, aN, cmp); return; }
        shift = shift < 0 ? 0 : shift;
        dif_t* const h = new dif_t[KEYS];
        for (int k = 0; k < KEYS; k++) h[k] = 0;
        B bi = b;
        for (RAI ai = a0; ai < aN; ++ai, ++bi) {
                const val_t& x = *ai;
                const int y = int((get_key(x) >> shift) & MASK);
                ++h[y];
                *bi = x;
        }
        for (int k = MASK; k > 0; k--) h[k - 1] += h[k];
        const RAI& a = a0;
        for (bi = b + N; bi > b;) {
                const val_t& x = *(--bi);
                const int y = int((get_key(x) >> shift) & MASK);
                const dif_t j = --h[y];
                a[j] = x;
        }
        if (shift > 0) {
                shift -= UNIT;
                for (int k = 0; k < MASK; k++)
                        msd_radix_sort_desc<UNIT>(shift,
                                a0 + h[k], a0 + h[k + 1], get_key, cmp, b);
                msd_radix_sort_desc<UNIT>(shift,
                        a0 + h[MASK], aN, get_key, cmp, b);
        }
        delete[] h;
}

#endif // msd_radix_sort_desc_h