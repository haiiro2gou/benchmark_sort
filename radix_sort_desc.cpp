#ifndef radix_sort_desc_cpp
#define radix_sort_desc_cpp

#include <iterator>
#include <vector>

template <int UNIT, int BITS, class RAI, class G>
void radix_sort_desc(RAI a0, RAI aN, G get_key)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        typedef typename G::result_type key_type;
        static const int KEYS = 1 << UNIT;
        static const int MASK = KEYS - 1;
        const dif_t N = aN - a0;
        const RAI& a = a0;

        if (N < 2) return;
        std::vector<dif_t> h(KEYS);
        std::vector<val_t> b(N);
        const typename std::vector<val_t>::iterator b0 = b.begin();
        const typename std::vector<val_t>::iterator bN = b.end();
        for (int shift = 0; shift < BITS; shift += UNIT) {
                for (int k = 0; k < KEYS; k++) h[k] = 0;
                typename std::vector<val_t>::iterator bi = b0;
                bool done = true;
                for (RAI ai = a0; ai < aN; ++ai, ++bi) {
                        const val_t& x = *ai;
                        const key_type y = get_key(x) >> shift;
                        if (y > 0) done = false;
                        ++h[int(y & MASK)];
                        *bi = x;
                }
                if (done) return;
                for (int k = MASK; k > 0; k--) h[k - 1] += h[k];
                for (bi = bN; bi > b0;) {
                        const val_t& x = *(--bi);
                        const int y = int((get_key(x) >> shift) & MASK);
                        const dif_t j = --h[y];
                        a[j] = x;
                }
        }
}

#endif // radix_sort_desc_cpp