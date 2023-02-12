#ifndef sort_launch_h
#define sort_launch_h

#include <chrono>

#include "bubble_sort.h"
#include "comb_sort.h"
#include "heap_sort1.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include "radix_sort.h"
#include "selection_sort.h"

#include "intro_sort.h"

typedef uint32_t key_type;

struct val_t {
    key_type key;
    double data;
};

inline std::ostream& operator<<(std::ostream& os, const val_t& x)
{
    os << "(" << x.key << ", " << x.data << ")";
    return os;
}

struct cmp_t : public std::binary_function<val_t,val_t,bool>
{
    bool operator()(const val_t& a, const val_t& b) const
    {
        return a.key < b.key;
    }
};

struct get_key_t : public std::unary_function<val_t,key_type>
{
    key_type operator()(const val_t& x) const { return x.key; }
};

using namespace std;

vector<double> sort_launch(vector<val_t> base) {
    // 変数設定
    vector<val_t> vec;
    chrono::system_clock::time_point start, end;
    vector<double> estimated(10);
    bool suc;
    vector<val_t> sorted = base;
    sort(sorted.begin(), sorted.end(), cmp_t());

    for (int i = 0; i < 10; i++) {
        // 例外処理(低速化回避)
        if ((i == 1 || i == 4 || i == 8) && base.size() > 10000) {
            estimated[i] = -1;
            continue;
        }
        
        // 処理呼び出し
        vec = base;
        start = chrono::system_clock::now();
        switch (i) {
            case 0:
                sort(vec.begin(), vec.end(), cmp_t());
                break;
            case 1:
                bubble_sort(vec.begin(), vec.end(), cmp_t());
                break;
            case 2:
                comb_sort(vec.begin(), vec.end(), cmp_t());
                break;
            case 3:
                heap_sort1(vec.begin(), vec.end(), cmp_t());
                break;
            case 4:
                insertion_sort(vec.begin(), vec.end(), cmp_t());
                break;
            case 5:
                merge_sort(vec.begin(), vec.end(), cmp_t());
                break;
            case 6:
                quick_sort(vec.begin(), vec.end(), cmp_t());
                break;
            case 7:
                radix_sort<8,32>(vec.begin(), vec.end(), get_key_t());
                break;
            case 8:
                selection_sort(vec.begin(), vec.end(), cmp_t());
                break;
            case 9:
                intro_sort(vec.begin(), vec.end(), cmp_t());
                break;
            default:
                fprintf(stderr, "\033[0KError has been occurred!: Null Pointer Exception\n");
                break;
        }
        end = chrono::system_clock::now();
        estimated[i] = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/100000.0);
        suc = true;
        for (int i = 0; i < base.size(); i++) { if (vec[i].key != sorted[i].key) suc = false; }
        if (!suc) estimated[i] = -1;
        if (!suc) {
            fprintf(stderr, "\033[0KError has been occurred!: Sort Failure\n");
        }
    }

    return estimated;
}

#endif