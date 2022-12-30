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
#include "library/bubble_sort.cpp"
#include "library/comb_sort.cpp"
#include "library/heap_sort1.cpp"
#include "library/insertion_sort.cpp"
#include "library/merge_sort.cpp"
#include "library/quick_sort.cpp"
#include "library/radix_sort.cpp"
#include "library/selection_sort.cpp"

#include "library/intro_sort.cpp"

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

    auto vec = a, sorted = a;
    std::sort(sorted.begin(), sorted.end(), cmp_t());
    chrono::system_clock::time_point start, end;
    double estimated;
    bool suc;
    map<string,vector<string>> res;

    #pragma omp parallel sections private(vec, start, end, estimated, suc) shared(a, res)
    {
        #pragma omp section
        {
            //* default sort
            vec = a;
            start = chrono::system_clock::now();
            std::sort(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Default"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* bubble sort
            vec = a;
            start = chrono::system_clock::now();
            bubble_sort(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Bubble"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* comb sort
            vec = a;
            start = chrono::system_clock::now();
            comb_sort(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Comb"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* heap sort
            vec = a;
            start = chrono::system_clock::now();
            heap_sort1(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Heap"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* insertion sort
            vec = a;
            start = chrono::system_clock::now();
            insertion_sort(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Insertion"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* merge sort
            vec = a;
            start = chrono::system_clock::now();
            merge_sort(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Merge"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* quick sort
            vec = a;
            start = chrono::system_clock::now();
            quick_sort(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Quick"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* radix sort
            vec = a;
            start = chrono::system_clock::now();
            radix_sort<8, 32>(vec.begin(), vec.end(), get_key_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Radix"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* selection sort
            vec = a;
            start = chrono::system_clock::now();
            selection_sort(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            bool suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false;}
            res["Selection"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }

        #pragma omp section
        {
            //* intro sort
            vec = a;
            start = chrono::system_clock::now();
            intro_sort(vec.begin(), vec.end(), cmp_t());
            end = chrono::system_clock::now();
            estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
            suc = true;
            for (int i = 0; i < N; ++i) { if (vec[i].key != sorted[i].key) suc = false; }
            res["Intro"].push_back(suc ? to_string(estimated) : "Failed!");
            // */
        }
    }

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