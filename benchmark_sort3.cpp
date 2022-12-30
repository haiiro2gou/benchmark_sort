#include <chrono>
#include <climits>
#include <cmath>
#include <fstream>
#include <numeric>
#include <omp.h>
#include <random>

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

const int inf = INT_MAX / 2;

inline double round(double x, int p) {
    int64_t xp10 = pow(10, p);
    return double(round(x * xp10)) / xp10;
}

std::vector<val_t> get_rand_vector(int size) {
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::vector<val_t> random_vector(size);
    for (int i = 0; i < size; i++) {
        random_vector[i].key = engine();
        random_vector[i].data = i;
    }
    return random_vector;
}

int N, M;
void input() {
    printf("N> ");
    scanf("%d", &N);
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
    scanf("%d", &M);
    if (M > 30) {
        M = 30;
        printf("M is too big, so M has been set to 30.\n");
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
        log_10[i] = round(pow(10, 1.0 * (i+1) / 10 - 1), 2);
    }

    // 処理部
    vector<vector<double>> data(N*10, vector<double>(10));

    int cur = 0;
    int mx = int(pow(10, N));
    for (int xp10 = 10; xp10 <= mx; xp10 *= 10) {
        for (auto x : log_10) {
            int size = x * xp10;
            vector<double> res[10];

            string progress = "";
            for (int i = 0; i <= size; i += mx / 20) { progress += "#"; }
            fprintf(stderr, "[%-20s] %4.2lf%% (%d/%d)\r", progress.c_str(), 100.0*size/mx, size, mx);

            for (int i = 0; i < M; i++) {
                vector<val_t> base = get_rand_vector(size);
                auto sorted = base;
                sort(sorted.begin(), sorted.end(), cmp_t());

                vector<val_t> vec;
                chrono::system_clock::time_point start, end;
                double estimated;
                bool suc;
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
        }
    }
    fprintf(stderr,"\nfinish!\n");

    // 出力部
    string path = "./result.csv";
    ofstream ofs(path);
    if (ofs) {
        // ofs << "Default,Bubble,Comb,Heap,Insertion,Merge,Quick,Radix,Selection,Intro" << endl;
        for (size_t i = 0; i < data.size(); i++) {
            for (size_t j = 0; j < data[i].size(); j++) { ofs << data[i][j] << (j < data[i].size() - 1 ? "," : "\n"); }
        }
    }

    return 0;
}