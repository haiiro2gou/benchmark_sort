#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <chrono>
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
    static const int N = 1e2;
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

    auto vec = a;
    chrono::system_clock::time_point start, end;
    double estimated;

    //* default sort
    vec = a;
    start = chrono::system_clock::now();
    std::sort(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("  Default Sort: %13.6lf ms\n", estimated);
    // */

    //* bubble sort
    vec = a;
    start = chrono::system_clock::now();
    bubble_sort(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("   Bubble Sort: %13.6lf ms\n", estimated);
    // */

    //* comb sort
    vec = a;
    start = chrono::system_clock::now();
    comb_sort(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("     Comb Sort: %13.6lf ms\n", estimated);
    // */

    //* heap sort
    vec = a;
    start = chrono::system_clock::now();
    heap_sort1(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("     Heap Sort: %13.6lf ms\n", estimated);
    // */

    //* insertion sort
    vec = a;
    start = chrono::system_clock::now();
    insertion_sort(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("Insertion Sort: %13.6lf ms\n", estimated);
    // */

    //* merge sort
    vec = a;
    start = chrono::system_clock::now();
    merge_sort(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("    Merge Sort: %13.6lf ms\n", estimated);
    // */

    //* quick sort
    vec = a;
    start = chrono::system_clock::now();
    quick_sort(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("    Quick Sort: %13.6lf ms\n", estimated);
    // */

    //* radix sort
    vec = a;
    start = chrono::system_clock::now();
    radix_sort<8, 32>(vec.begin(), vec.end(), get_key_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("    Radix Sort: %13.6lf ms\n", estimated);
    // */

    //* selection sort
    vec = a;
    start = chrono::system_clock::now();
    selection_sort(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("Selection Sort: %13.6lf ms\n", estimated);
    // */

    //* intro sort
    vec = a;
    start = chrono::system_clock::now();
    intro_sort(vec.begin(), vec.end(), cmp_t());
    end = chrono::system_clock::now();
    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
    printf("    Intro Sort: %13.6lf ms\n", estimated);
    // */

    /*
    for (int i = 0; i < N; i++) {
        cout << vec[i].key;
        if (i == N - 1) cout << endl;
        else cout << " ";
    }
    // */
    return 0;
}