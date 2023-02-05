#ifndef bucket_sort_h
#define bucket_sort_h

#include <iterator>
#include <vector>

template <int M, class FI, class G>
void bucket_sort(FI a0, FI aN, G get_key)
{
        typedef typename std::iterator_traits<FI>::value_type val_t;

        if (a0 == aN) return;
        std::vector< std::vector<val_t> > bucket(M + 1);
        FI ai;
        for (ai = a0; ai != aN; ++ai) {
                const val_t& x = *ai;
                bucket[get_key(x)].push_back(x);
        }
        ai = a0;
        for (int j = 0; j <= M; j++) {
                std::vector<val_t>& bj = bucket[j];
                ai = std::copy(bj.begin(), bj.end(), ai);
        }
}

#endif // bucket_sort_h