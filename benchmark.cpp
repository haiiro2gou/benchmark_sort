//* benchmark.cpp
// 特定の要素についてランダムなデータを生成し、ソート処理に要した時間を統計して出力する。
// */

#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <string>

typedef uint32_t key_type;
struct val_t {
    key_type key;
    std::string data;
};

typedef struct {
    int current;
    int max;
} prog_bar;

int input(std::string name, std::string desc, int min, int max) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int res;
    bool check = false;
    fprintf(stderr, "Please input %s. (%d ~ %d)\n(%s)\n", name.c_str(), min, max, desc.c_str());
    while (!check) {
        fprintf(stderr, "\r\033[0K%s> ", name.c_str());
        std::cin >> res;
        if (res >= min && res <= max) check = true;
        else {
            fprintf(stderr, "\r\033[1A\r\033[0KInvalid input! Input is allowed to range from %d ~ %d.\n", min, max);
        }
    }
    fprintf(stderr, "\r\033[1A\r\033[0K\r\033[1A\r\033[0K\r\033[1A\r\033[0K");
    return res;
}

std::string time_elapsed(std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end) {
    int elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    int seconds = elapsed / 1000;
    int minutes = seconds / 60;
    int hours = minutes / 60;
    elapsed %= 1000;
    seconds %= 60;
    minutes %= 60;
    return (seconds < 1 ? std::to_string(elapsed) + "ms" : (minutes < 1 ? "" : (hours < 1 ? "" : std::to_string(hours) + "h ") + std::to_string(minutes) + "m ") + std::to_string(seconds) + (minutes < 1 ? "." + std::to_string(elapsed / 100) : "") + "s");
}

void bar_create(prog_bar conf) {
    fprintf(stderr, "[..............................] 0 / %ld (0.0 %)\n", conf.max);
}

void bar_update(prog_bar conf) {
    double perc = 100.0 * conf.current / conf.max;
    std::string bar = "";
    for (int i = 0; i < 30; i++) {
        if (conf.current * 30 >= conf.max * (i + 1)) bar += '#';
        else bar += '.';
    }
    fprintf(stderr, "\r\033[1A\r\033[0K[%s] %ld / %ld (%.1lf %)\n", bar.c_str(), conf.current, conf.max, perc);
}

void bar_delete() {
    fprintf(stderr, "\r\033[1A\r\033[0K");
}

const int inf = INT_MAX / 2;
int main() {
    using namespace std;

    fprintf(stderr, "Sort Bench v0.2.0-beta\n");
    fprintf(stderr, "Made by @haiiro2gou\n");
    fprintf(stderr, "\n");

    // input phase
    int n, size; string type;
    {
        bool ipcheck = false;
        while (!ipcheck) {
            int T = input("Type", "Determines the type of sorting that will take place during benchmarking (1: Number, 2: Size)", 1, 2);
            type = (T == 1 ? "Number" : "Size");
            int N = input("N", "Determines the number of elements to be sorted", 0, 7);
            n = pow(10, N);
            if (T == 2) {
                int S = input("Size", "Specifies the size of the data to be sorted", 1, 7);
                size = pow(10, S);
            }
            else size = 1000;
            
            fprintf(stderr, "Your input was...\n");
            fprintf(stderr, "Type: %s\n", type.c_str());
            fprintf(stderr, "Elements: %ld\n", n);
            if (T == 2) fprintf(stderr, "Size: %ld\n", size);

            ipcheck = input("Check", "Is this input correct? (Yes: 1 / No: 0)", 0, 1);

            if (!ipcheck) {
                fprintf(stderr, "\r\033[1A\r\033[0K\r\033[1A\r\033[0K\r\033[1A\r\033[0K");
                if (T == 2) fprintf(stderr, "\r\033[1A\r\033[0K");
            }
            else fprintf(stderr, "\n");
        }
    }

    chrono::system_clock::time_point proc_start = chrono::system_clock::now();

    // data creation phase
    vector<val_t> list(n);
    {
        fprintf(stderr, "Data generation has started...\n");
        
        chrono::system_clock::time_point gen_start = chrono::system_clock::now();
        string abc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        prog_bar gen_1 = {0, n};
        bar_create(gen_1);
        chrono::system_clock::time_point gen_1_cp = gen_start;
        for (int i = 0; i < n; i++) {
            gen_1.current++;
            chrono::system_clock::time_point gen_1_now = chrono::system_clock::now();
            if (static_cast<int>(chrono::duration_cast<chrono::milliseconds>(gen_1_now - gen_1_cp).count() / 200.0)) {
                bar_update(gen_1);
                gen_1_cp = gen_1_now;
            }
            prog_bar gen_2 = {0, size};
            bar_create(gen_2);
            chrono::system_clock::time_point gen_2_cp = gen_1_now;

            random_device seed_gen;
            mt19937 engine(seed_gen());
            list[i].key = engine() % inf;
            list[i].data = "";
            for (int j = 0; j < size; j++) {
                gen_2.current++;
                chrono::system_clock::time_point gen_2_now = chrono::system_clock::now();
                if (static_cast<int>(chrono::duration_cast<chrono::milliseconds>(gen_2_now - gen_2_cp).count() / 200.0)) {
                    bar_update(gen_2);
                    gen_2_cp = gen_2_now;
                }

                list[i].data += abc[engine() % 52];
            }
            bar_delete();
        }
        bar_delete();
        chrono::system_clock::time_point gen_end = chrono::system_clock::now();

        fprintf(stderr, "Data generation has completed! (Elapsed: %s)\n", time_elapsed(gen_start, gen_end).c_str());
    }

    // main phase

    return 0;
}