#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <chrono>
#include <map>
#include <omp.h>
#include <unistd.h>
#include "bubble_sort.cpp"
#include "comb_sort.cpp"
#include "heap_sort1.cpp"
#include "insertion_sort.cpp"
#include "merge_sort.cpp"
#include "quick_sort.cpp"
#include "radix_sort.cpp"
#include "selection_sort.cpp"

#include "intro_sort.cpp"

typedef int key_type;

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

int main() {
    using namespace std;
    printf("N> ");
    static int N; cin >> N;
    vector<val_t> a(N);

    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        a[i].key = rand();
        a[i].data = i;
    }
    /*
    for (int i = 0; i < N; i++) {
        cout << a[i].key;
        if (i == N - 1) cout << endl;
        else cout << " ";
    }
    // */

    /* omp check
    printf("Max Threads: %d\n", omp_get_max_threads());
    // */

    auto vec = a;
    sort(vec.begin(), vec.end(), cmp_t());
    chrono::system_clock::time_point start, end;
    double estimated;
    map<string,vector<string>> res;

    #pragma omp parallel sections private(start,end)
    {
        #pragma omp section
        {
            //* default sort
            auto default_vec = a;
            start = chrono::system_clock::now();
            std::sort(default_vec.begin(), default_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool default_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != default_vec[i].key) default_suc = false; }
            res["Default"].push_back(default_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* bubble sort
            auto bubble_vec = a;
            start = chrono::system_clock::now();
            bubble_sort(bubble_vec.begin(), bubble_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool bubble_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != bubble_vec[i].key) bubble_suc = false; }
            res["Bubble"].push_back(bubble_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* comb sort
            auto comb_vec = a;
            start = chrono::system_clock::now();
            comb_sort(comb_vec.begin(), comb_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool comb_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != comb_vec[i].key) comb_suc = false; }
            res["Comb"].push_back(comb_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* heap sort
            auto heap_vec = a;
            start = chrono::system_clock::now();
            heap_sort1(heap_vec.begin(), heap_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool heap_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != heap_vec[i].key) heap_suc = false; }
            res["Heap"].push_back(heap_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* insertion sort
            auto insertion_vec = a;
            start = chrono::system_clock::now();
            insertion_sort(insertion_vec.begin(), insertion_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool insertion_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != insertion_vec[i].key) insertion_suc = false; }
            res["Insertion"].push_back(insertion_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* merge sort
            auto merge_vec = a;
            start = chrono::system_clock::now();
            merge_sort(merge_vec.begin(), merge_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool merge_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != merge_vec[i].key) merge_suc = false; }
            res["Merge"].push_back(merge_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* quick sort
            auto quick_vec = a;
            start = chrono::system_clock::now();
            quick_sort(quick_vec.begin(), quick_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool quick_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != quick_vec[i].key) quick_suc = false; }
            res["Quick"].push_back(quick_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* radix sort
            auto radix_vec = a;
            start = chrono::system_clock::now();
            radix_sort<8, 32>(radix_vec.begin(), radix_vec.end(), get_key_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool radix_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != radix_vec[i].key) radix_suc = false; }
            res["Radix"].push_back(radix_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* selection sort
            auto selection_vec = a;
            start = chrono::system_clock::now();
            selection_sort(selection_vec.begin(), selection_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool selection_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != selection_vec[i].key) selection_suc = false;}
            res["Selection"].push_back(selection_suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* intro sort
            auto intro_vec = a;
            start = chrono::system_clock::now();
            intro_sort(intro_vec.begin(), intro_vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool intro_suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != intro_vec[i].key) intro_suc = false; }
            res["Intro"].push_back(intro_suc ? to_string(estimated) : "Failed!");
            // */
        }
    }

    sleep(1);
    /*
    for (int i = 0; i < N; i++) {
        cout << vec[i].key;
        if (i == N - 1) cout << endl;
        else cout << " ";
    }
    // */
    for (auto itr = res.begin(); itr != res.end(); ++itr) {
        for (auto str : itr->second) {
            printf("%9s Sort: %13s ms\n", itr->first.c_str(), str.c_str());
        }
    }
    return 0;
}