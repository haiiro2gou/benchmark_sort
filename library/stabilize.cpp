#include <iterator>
#include <vector>
#include "pquick_sort.cpp"

template <class val_t>
struct stabilizing_wrapper
{
    val_t val;
    int index;

    stabilizing_wrapper() : val(), index(0) {}
    stabilizing_wrapper(const val_t& v, int i) : val(v), index(i) {}
};

template <class val_t, class C>
struct stabilizing_comparator
{
    typedef stabilizing_wrapper<val_t> val_t2;
    C cmp;

    stabilizing_comparator(C c) : cmp(c) {}

    bool operator()(const val_t2& a, const val_t2& b)
    {
        if (cmp(a.val, b.val)) return true;
        if (cmp(b.val, a.val)) return false;
        if (a.index < b.index) return true;
        return false;
    }
};

template <class RAI, class C>
void spquick_sort(RAI a0, RAI aN, C cmp)
{
    typedef typename std::iterator_traits<RAI>::value_type val_t;
    typedef stabilizing_wrapper<val_t> val_t2;
    const int N = std::distance(a0, aN);
    stabilizing_comparator<val_t,C> cmp2(cmp);
    std::vector<val_t2> b(N);
    const RAI a = a0;
    for (int i = 0; i < N; i++) b[i] = val_t2(a[i], i);
    pquick_sort(b.begin(), b.end(), cmp2);
    for (int i = 0; i < N; i++) a[i] = b[i].val;
}