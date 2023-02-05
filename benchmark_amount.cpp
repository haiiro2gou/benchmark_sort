#include <chrono>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <random>

#include "library/bubble_sort.h"
#include "library/comb_sort.h"
#include "library/heap_sort1.h"
#include "library/insertion_sort.h"
#include "library/merge_sort.h"
#include "library/quick_sort.h"
#include "library/radix_sort.h"
#include "library/selection_sort.h"

#include "library/intro_sort.h"

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

inline double round(double x, int p) {
    int64_t xp10 = pow(10, p);
    return double(round(x * xp10)) / xp10;
}

const int inf = INT_MAX / 2;

std::vector<val_t> get_rand_vector(int size, int limit = inf) {
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::vector<val_t> random_vector(size);
    for (int i = 0; i < size; i++) {
        random_vector[i].key = engine() % limit;
        random_vector[i].data = i;
    }
    return random_vector;
}

int N, M;
void input() {
    printf("N> ");
    std::cin >> N;
    if (N > 7) {
        N = 7;
        printf("N is too big, so N has been set to 7.\n");
        fflush(stdout);
    }
    if (N < 1) {
        N = 1;
        printf("N is too small, so N has been set to 1.\n");
        fflush(stdout);
    }
    printf("M> ");
    std::cin >> M;
    if (M > 1000) {
        M = 1000;
        printf("M is too big, so M has been set to 1000.\n");
        fflush(stdout);
    }
    if (M < 1) {
        M = 1;
        printf("M is too small, so M has been set to 1.\n");
        fflush(stdout);
    }
}

int main() {
    using namespace std;

    // 設定部
    input();
    vector<double> log_10(10);
    for (int i = 0; i < 10; i++) {
        log_10[i] = pow(10, 1.0 * (i+1) / 10 - 1);
    }

    // 処理部
    vector<vector<double>> data(N*10, vector<double>(10));

    chrono::system_clock::time_point all_start = chrono::system_clock::now();

    int cur = 0;
    vector<int> stack;
    int mx = int(pow(10, N));
    for (int xp10 = 10; xp10 <= mx; xp10 *= 10) {
        for (auto x : log_10) {
            int size = x * xp10;
            /* stack check
            if (!binary_search(stack.begin(), stack.end(), size)) stack.push_back(size);
            else continue;
            //*/
            vector<double> res[10];

            // 進捗バー1
            string progress1 = "";
            for (int i = 0; i < 30; i++) {
                if (size * 30 >= i * mx) progress1 += "#";
                else break;
            }
            fprintf(stderr, "[%-30s] %6.2lf%% (%d/%d)\n", progress1.c_str(), 100.0*size/mx, size, mx);

            for (int i = 0; i < M; i++) {
                // 進捗バー2
                string progress2 = "";
                for (int j = 0; j < 30; j++) {
                    if (i * 30 >= j * M) progress2 += "#";
                    else break;
                }
                fprintf(stderr, "[%-30s] %6.2lf%% (%d/%d)\r", progress2.c_str(), 100.0*(i+1)/M, i+1, M);

                vector<val_t> base = get_rand_vector(size, size);
                auto sorted = base;
                sort(sorted.begin(), sorted.end(), cmp_t());

                vector<val_t> vec;
                chrono::system_clock::time_point start, end;
                double estimated;
                bool suc;
                #pragma omp parallel sections private(vec, start, end, estimated, suc) shared (base, res)
                {
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
                }
            }

            vector<bool> check(10, true);
            for (int i = 0; i < 10; i++) {
                for (auto x : res[i]) {
                    if (x == -1) check[i] = false;
                }
            }

            for (int i = 0; i < 10; i++) {
                if (check[i]) data[cur][i] = accumulate(res[i].begin(), res[i].end(), 0.0) / res[i].size();
                else data[cur][i] = -1;
            }
            cur++;
            
            // 一周終わり
            fprintf(stderr, "\033[K\033[1A");
        }
    }
    chrono::system_clock::time_point all_end = chrono::system_clock::now();
    int all_estimated = static_cast<int>(chrono::duration_cast<chrono::milliseconds>(all_end-all_start).count()/1000.0);
    fprintf(stderr,"\033[Kfinish! (");
    if (all_estimated/(24*60*60)) fprintf(stderr, "%dd %02dh %02dm %02ds", all_estimated/(24*60*60), all_estimated/(60*60)%24, all_estimated/60%60, all_estimated%60);
    else if (all_estimated/(60*60)) fprintf(stderr, "%dh %02dm %02ds", all_estimated/(60*60)%24, all_estimated/60%60, all_estimated%60);
    else if (all_estimated/60) fprintf(stderr, "%02dm %02ds", all_estimated/60%60, all_estimated%60);
    else if (all_estimated) fprintf(stderr, "%ds", all_estimated%60);
    else fprintf(stderr, "<1s");
    fprintf(stderr, ")\n");

    // 出力部
    string path = "./result/amount.csv";
    ofstream ofs(path);
    if (ofs) {
        // ofs << "Default,Bubble,Comb,Heap,Insertion,Merge,Quick,Radix,Selection,Intro" << endl;
        for (size_t i = 0; i < data.size(); i++) {
            for (size_t j = 0; j < data[i].size(); j++) {
                char str[20];
                sprintf(str, "%.9lf", data[i][j]);
                ofs << str << (j < data[i].size() - 1 ? "," : "\n");
            }
        }
    }

    return 0;
}