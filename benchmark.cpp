//* benchmark.cpp
// 特定の要素についてランダムなデータを生成し、ソート処理に要した時間を統計して出力する。
// */

#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
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
    int milli_second = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    int second = milli_second / 1000; milli_second %= 1000;
    int minute = second / 60; second %= 60;
    int hour = minute / 60; minute %= 60;
    std::string ret = "";
    if (hour) ret += to_string(hour) + "h ";
    if (minute || hour) ret += to_string(minute) + "m ";
    if (second && (minute || hour)) ret += to_string(second) + "s";
    if (second && !(minute || hour)) ret += to_string(second) + "." + to_string(milli_second / 100) + "s";
    if (!(second || minute || hour)) ret += to_string(milli_second) + "ms";
    return ret;
}

std::chrono::system_clock::time_point latest;
void bar_create(prog_bar conf) {
    fprintf(stderr, "%s 0 / %ld (0ms)\n[..............................] 0.0 %\n", conf.title.c_str(), conf.max);
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

std::vector<int> get_candidate(int start, int end) {
    std::vector<int> ret((end - start) * 10);
    for (int i = start; i < end; i++) {
        for (int j = 1; j <= 10; j++) {
            ret[i * 10 + j - 1] = round(pow(10, 1.0 * j / 10 + i));
        }
    }
    return ret;
}

const int inf = INT_MAX / 2;
int main() {
    using namespace std;

    fprintf(stderr, "Sort Bench v0.2.1\n");
    fprintf(stderr, "Made by @haiiro2gou\n");
    fprintf(stderr, "\n");

    // input phase
    int n, size, a; string type;
    {
        bool ipcheck = false;
        while (!ipcheck) {
            int T = input("Type", "Determines the type of sorting that will take place during benchmarking (1: Element, 2: Size)", 1, 2);
            type = (T == 1 ? "element" : "size");
            if (T == 1) {
                int N = input("N", "Determines the number of elements to be sorted", 1, 7);
                n = pow(10, N);
                size = 1000;
            }
            if (T == 2) {
                n = 1000;
                int S = input("Size", "Specifies the size of the data to be sorted", 1, 7);
                size = pow(10, S);
            }
            int A = input("Multiplier", "Determines the number of attempts.", 0, 4);
            a = pow(10, A);
            
            fprintf(stderr, "Your input was...\n");
            fprintf(stderr, "Type: %s\n", type.c_str());
            fprintf(stderr, "Elements: %ld\n", n);
            fprintf(stderr, "Size: %ld\n", size);
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
    vector<int> target = get_candidate(0, round(log10(type == "element" ? n : size)));
    vector<vector<vector<double>>> result(target.size(), vector<vector<double>>(a, vector<double>(10)));
    {
        fprintf(stderr, "Main Process has started...\n");

        const std::string abc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        random_device seed_gen;

        chrono::system_clock::time_point main_start = chrono::system_clock::now();
        int marker = 0;
        vector<val_t> list(n);

        prog_bar main = {"Main Loop", 0, (type == "element" ? n : size), chrono::system_clock::now()};
        bar_create(main);
        for (auto &t : target) {
            main.current = t;
            bar_update(main, true);

            list.resize(t);

            prog_bar l1 = {"Attempts", 0, a, chrono::system_clock::now()};
            bar_create(l1);
            for (int i = 0; i < a; i++) {
                l1.current++;
                bar_update(l1, true);

                // main sort phase
                prog_bar gen = {"Randomizer", 0, (type == "element" ? t : n), chrono::system_clock::now()};
                bar_create(gen);
                for (int g1 = 0; g1 < (type == "element" ? t : n); g1++) {
                    gen.current++;
                    bar_update(gen);

                    mt19937 engine(seed_gen());
                    list[i].key = engine() % inf;
                    list[i].data = "";
                    for (int g2 = 0; g2 < (type == "element" ? size : t); g2++) {
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

        chrono::system_clock::time_point output_start = chrono::system_clock::now();
        
        int c = round(log10((type == "element" ? n : size))) * 10;
        string path = "./result/" + type + "_";
        for (int i = 0; i < c; i++) {
            string path = "./result/" + type + "/" + to_string((int) round(pow(10, 0.1 * (i + 1)))) + ".csv";
            ofstream ofs(path);
            if (ofs) {
                for (int j = 0; j < a; j++) {
                    for (int k = 0; k < 10; k++) ofs << result[i][j][k] << (k < 9 ? ',' : '\n');
                }
            }
        }

        chrono::system_clock::time_point output_end = chrono::system_clock::now();

        fprintf(stderr, "\r\033[1A\r\033[0KBenchmark results have been exported. (Elapsed: %s)\n", time_elapsed(output_start, output_end).c_str());
    }

    return 0;
}
