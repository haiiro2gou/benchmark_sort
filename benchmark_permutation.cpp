#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <omp.h>

#include "library/bubble_sort.cpp"
#include "library/comb_sort.cpp"
#include "library/heap_sort1.cpp"
#include "library/insertion_sort.cpp"
#include "library/merge_sort.cpp"
#include "library/quick_sort.cpp"
#include "library/radix_sort.cpp"
#include "library/selection_sort.cpp"

#include "library/intro_sort.cpp"

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

int N;
void input() {
    printf("N> ");
    std::cin >> N;
    if (N > 10) {
        N = 10;
        printf("N is too big, so N has been set to 10.\n");
    }
    if (N < 1) {
        N = 1;
        printf("N is too small, so N has been set to 1.\n");
    }
}

int factorial(int x) {
    if (x < 0) return -1;
    if (x <= 1) return 1;
    return x * factorial(x-1);
}

int main() {
    using namespace std;

    // 設定部
    input();
    int total = 0;
    for (int i = 0; i < N; i++) {
        total += factorial(i+1);
    }
    

    // 処理部
    int cur = 0;
    vector<vector<double>> data;

    chrono::system_clock::time_point all_start = chrono::system_clock::now();
    for (int i = 0; i < N; i++) {
        vector<int> key(i+1);
        for (int j = 0; j < i+1; j++) {
            key[j] = j;
        }

        int size = i+1;
        do {
            vector<val_t> base(i+1);
            for (int j = 0; j < i+1; j++) {
                base[j].key = key[j];
                base[j].data = j;
            }

            vector<val_t> vec, sorted = base;
            sort(sorted.begin(), sorted.end(), cmp_t());
            chrono::system_clock::time_point start, end;
            double estimated;
            bool suc;
            vector<double> res[10];
            
            #pragma omp parallel sections private(vec, start, end, estimated, suc) shared (base, res)
            {
                //* default sort
                #pragma omp section
                {
                    vec = base;
                    start = chrono::system_clock::now();
                    sort(vec.begin(), vec.end(), cmp_t());
                    end = chrono::system_clock::now();
                    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
                    suc = true;
                    for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                    res[0].push_back(suc ? estimated : -1);
                }
                //*/

                //* comb sort
                #pragma omp section
                {
                    vec = base;
                    start = chrono::system_clock::now();
                    comb_sort(vec.begin(), vec.end(), cmp_t());
                    end = chrono::system_clock::now();
                    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
                    suc = true;
                    for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                    res[2].push_back(suc ? estimated : -1);
                }
                //*/

                //* heap sort
                #pragma omp section
                {
                    vec = base;
                    start = chrono::system_clock::now();
                    heap_sort1(vec.begin(), vec.end(), cmp_t());
                    end = chrono::system_clock::now();
                    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
                    suc = true;
                    for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                    res[3].push_back(suc ? estimated : -1);
                }
                //*/

                //* merge sort
                #pragma omp section
                {
                    vec = base;
                    start = chrono::system_clock::now();
                    merge_sort(vec.begin(), vec.end(), cmp_t());
                    end = chrono::system_clock::now();
                    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
                    suc = true;
                    for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                    res[5].push_back(suc ? estimated : -1);
                }
                //*/

                //* quick sort
                #pragma omp section
                {
                    vec = base;
                    start = chrono::system_clock::now();
                    quick_sort(vec.begin(), vec.end(), cmp_t());
                    end = chrono::system_clock::now();
                    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
                    suc = true;
                    for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                    res[6].push_back(suc ? estimated : -1);
                }
                //*/

                //* radix sort
                #pragma omp section
                {
                    vec = base;
                    start = chrono::system_clock::now();
                    radix_sort<8, 32>(vec.begin(), vec.end(), get_key_t());
                    end = chrono::system_clock::now();
                    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
                    suc = true;
                    for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                    res[7].push_back(suc ? estimated : -1);
                }
                //*/

                //* intro sort
                #pragma omp section
                {
                    vec = base;
                    start = chrono::system_clock::now();
                    intro_sort(vec.begin(), vec.end(), cmp_t());
                    end = chrono::system_clock::now();
                    estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0);
                    suc = true;
                    for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                    res[9].push_back(suc ? estimated : -1);
                }
                //*/

                //* bubble sort
                #pragma omp section
                {
                    if (size > 10000) { res[1].push_back(-1); }
                    else {
                        vec = base;
                        start = chrono::system_clock::now();
                        bubble_sort(vec.begin(), vec.end(), cmp_t());
                        end = chrono::system_clock::now();
                        estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/100000.0);
                        suc = true;
                        for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                        res[1].push_back(suc ? estimated : -1);
                    }
                }
                //*/

                //* insertion sort
                #pragma omp section
                {
                    if (size > 10000) { res[4].push_back(-1); }
                    else {
                        vec = base;
                        start = chrono::system_clock::now();
                        bubble_sort(vec.begin(), vec.end(), cmp_t());
                        end = chrono::system_clock::now();
                        estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/100000.0);
                        suc = true;
                        for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                        res[4].push_back(suc ? estimated : -1);
                    }
                }
                //*/

                //* selection sort
                #pragma omp section
                {
                    if (size > 10000) { res[8].push_back(-1); }
                    else {
                        vec = base;
                        start = chrono::system_clock::now();
                        bubble_sort(vec.begin(), vec.end(), cmp_t());
                        end = chrono::system_clock::now();
                        estimated = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end-start).count()/100000.0);
                        suc = true;
                        for (int i = 0; i < size; i++) { if (vec[i].key != sorted[i].key) suc = false; }
                        res[8].push_back(suc ? estimated : -1);
                    }
                }
                //*/
            }

            vector<double> ret(10, -1);
            for (int i = 0; i < 10; i++) {
                if (res[i].front() != -1) ret[i] = res[i].front();
            }
            data.push_back(ret);
            cur++;
            string progress = "";
            for (int i = 0; i < 30; i++) {
                if (cur * 30 > total * i) progress += '#';
            }
            fprintf(stderr, "[%-30s] (%d/%d)\r", progress.c_str(), cur, total);
        } while (next_permutation(key.begin(), key.end()));
    }
    chrono::system_clock::time_point all_end = chrono::system_clock::now();
    int all_estimated = static_cast<int>(chrono::duration_cast<chrono::milliseconds>(all_end-all_start).count()/1000.0);
    fprintf(stderr,"\033[Kfinish! (");
    if (all_estimated/(24*60*60)) fprintf(stderr, "%dd %02dh %02dm %02ds", all_estimated/(24*60*60), all_estimated/(60*60)%24, all_estimated/60%60, all_estimated%60);
    else if (all_estimated/(60*60)) fprintf(stderr, "%dh %02dm %02ds", all_estimated/(60*60)%24, all_estimated/60%60, all_estimated%60);
    else if (all_estimated/60) fprintf(stderr, "%dm %02ds", all_estimated/60%60, all_estimated%60);
    else if (all_estimated) fprintf(stderr, "%ds", all_estimated%60);
    else fprintf(stderr, "<1s");
    fprintf(stderr, ")\n");

    // 出力部
    string path = "./result/permutation.csv";
    ofstream ofs(path);
    if (ofs) {
        for (int i = 0; i < cur; i++) {
            for (int j = 0; j < 10; j++) {
                char str[20];
                sprintf(str, "%.9lf", data[i][j]);
                ofs << str << (j < 9 ? ',' : '\n');
            }
        }
    }

    return 0;
}