#ifndef counting_sort_cpp
#define counting_sort_cpp

#include <iterator>
#include <vector>

template <int M, class RAI, class G>
void counting_sort(RAI a0, RAI aN, G get_key)
{
        typedef typename std::iterator_traits<RAI>::value_type val_t;
        typedef typename std::iterator_traits<RAI>::difference_type dif_t;
        const dif_t N = aN - a0;
        const RAI& a = a0;

        if (N < 2) return;
        std::vector<dif_t> h(M + 1, 0);
        std::vector<val_t> b(N);
        const typename std::vector<val_t>::iterator b0 = b.begin();
        const typename std::vector<val_t>::iterator bN = b.end();
        typename std::vector<val_t>::iterator bi = b0;
        for (RAI ai = a0; ai < aN; ++ai, ++bi) {
                const val_t& x = *ai;
                ++h[get_key(x)];
                *bi = x;
        }
        for (int k = 1; k <= M; k++) h[k] += h[k - 1];
        for (bi = bN; bi > b0;) {
                const val_t& x = *(--bi);
                const dif_t j = --h[get_key(x)];
                a[j] = x;
        }
}

#endif // counting_sort_cpp