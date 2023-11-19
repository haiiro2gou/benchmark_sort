//* benchmark.cpp
// 特定の要素についてランダムなデータを生成し、ソート処理に要した時間を統計して出力する。
// */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "library/sort_launch.h"

typedef struct {
    std::string title;
    int current;
    int max;
    std::chrono::system_clock::time_point start;
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

std::chrono::system_clock::time_point latest;
void bar_create(prog_bar conf) {
    fprintf(stderr, "%s 0 / %ld (0 ms)\n[..............................] 0.0 %\n", conf.title.c_str(), conf.max);
    latest = std::chrono::system_clock::now();
}

void bar_update(prog_bar conf, bool force = false) {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    if (static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(now - latest).count()) > 200 || force) {
        latest = std::chrono::system_clock::now();

        double perc = 100.0 * conf.current / conf.max;
        std::string bar = "";
        for (int i = 0; i < 30; i++) {
            if (conf.current * 30 >= conf.max * (i + 1)) bar += '#';
            else bar += '.';
        }

        fprintf(stderr, "\r\033[1A\r\033[0K\r\033[1A\r\033[0K%s %ld / %ld (%s)\n[%s] %.1lf %\n", conf.title.c_str(), conf.current, conf.max, time_elapsed(conf.start, now).c_str(), bar.c_str(), perc);
    }
}

void bar_delete() {
    fprintf(stderr, "\r\033[1A\r\033[0K\r\033[1A\r\033[0K");
}

std::vector<int> get_candidate(int digit) {
    std::vector<int> ret(digit * 10);
    for (int i = 0; i < digit; i++) {
        for (int j = 1; j <= 10; j++) {
            ret[i * 10 + j - 1] = round(pow(10, 1.0 * j / 10 + i));
        }
    }
    return ret;
}

const int inf = INT_MAX / 2;
int main() {
    using namespace std;

    fprintf(stderr, "Sort Bench v0.2.0-beta\n");
    fprintf(stderr, "Made by @haiiro2gou\n");
    fprintf(stderr, "\n");

    // input phase
    int n, size, a; string type;
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
            int A = input("Multiplier", "Determines the number of attempts.", 0, 4);
            a = pow(10, A);
            
            fprintf(stderr, "Your input was...\n");
            fprintf(stderr, "Type: %s\n", type.c_str());
            fprintf(stderr, "Elements: %ld\n", n);
            if (T == 2) fprintf(stderr, "Size: %ld\n", size);
            fprintf(stderr, "Attempts: %ld\n", a);

            ipcheck = input("Check", "Is this input correct? (Yes: 1 / No: 0)", 0, 1);

            if (!ipcheck) {
                fprintf(stderr, "\r\033[1A\r\033[0K\r\033[1A\r\033[0K\r\033[1A\r\033[0K\r\033[1A\r\033[0K");
                if (T == 2) fprintf(stderr, "\r\033[1A\r\033[0K");
            }
            else fprintf(stderr, "\n");
        }
    }

    // main phase
    {
        fprintf(stderr, "Main Process has started...\n");

        const std::string abc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        random_device seed_gen;

        chrono::system_clock::time_point main_start = chrono::system_clock::now();
        vector<int> target = get_candidate(round(log10(type == "Number" ? n : size)));
        vector<vector<vector<double>>> result(target.size(), vector<vector<double>>(a, vector<double>(10)));
        int marker = 0;

        auto main_st = chrono::system_clock::now();
        prog_bar main = {"Main Loop", 0, (type == "Number" ? n : size), main_st};
        bar_create(main);
        for (auto &t : target) {
            main.current = t;
            bar_update(main, true);

            prog_bar l1 = {"Attempts", 0, a, chrono::system_clock::now()};
            bar_create(l1);
            for (int i = 0; i < a; i++) {
                l1.current++;
                bar_update(l1, true);

                // main sort phase
                vector<val_t> list((type == "Number" ? t : n));
                prog_bar gen = {"Randomizer", 0, (type == "Number" ? t : n), chrono::system_clock::now()};
                bar_create(gen);
                for (int g1 = 0; g1 < (type == "Number" ? t : n); g1++) {
                    gen.current++;
                    bar_update(gen);

                    mt19937 engine(seed_gen());
                    list[i].key = engine() % inf;
                    list[i].data = "";
                    for (int g2 = 0; g2 < (type == "Number" ? size : t); g2++) {
                        list[i].data += abc[engine() % 52];
                    }
                }
                bar_delete();

                result[marker][i] = sort_launch(list);
            }
            bar_delete();

            marker++;
        }
        bar_delete();
        chrono::system_clock::time_point main_end = chrono::system_clock::now();

        fprintf(stderr, "\r\033[1A\r\033[0KMain Process has completed! (Elapsed: %s)\n", time_elapsed(main_start, main_end).c_str());
    }

    // output phase
    {
        fprintf(stderr, "Benchmark results are now being exported.\n");
    }

    return 0;
}