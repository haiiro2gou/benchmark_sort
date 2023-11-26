//* benchmark_amount.cpp
// 指定した要素数までのデータ列を生成し、処理時間の平均値を出力します
// */

#include <climits>
#include <cmath>
#include <iostream>
#include <random>

#include "library/csv_ctrl.h"
#include "library/deviation.h"
#include "library/progress_bar.h"
#include "library/sort_launch.h"

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
    fprintf(stderr, "\r\033[0KN> ");
    std::cin >> N;
    if (N > 7) {
        N = 7;
        fprintf(stderr, "\r\033[0KN is too big, so N has been set to 7.\n");
    }
    if (N < 1) {
        N = 1;
        fprintf(stderr, "\r\033[0KN is too small, so N has been set to 1.\n");
    }
    fprintf(stderr, "\r\033[0KM> ");
    std::cin >> M;
    if (M > 1000) {
        M = 1000;
        fprintf(stderr, "\r\033[0KM is too big, so M has been set to 1000.\n");
    }
    if (M < 1) {
        M = 1;
        fprintf(stderr, "\r\033[0KM is too small, so M has been set to 1.\n");
    }
}

int main() {
    using namespace std;

    // 設定部
    input();
    vector<double> log_10(10);
    for (int i = 0; i < 10; i++) {
        log_10[i] = pow(10, 1.0 * (i + 1) / 10 - 1);
    }
    string temp_path = "./temp/amount/.csv";
    string temp_progress_path = "./temp/amount/progress.csv";
    
    // csvチェック1
    bool check_temp = false;
    csv_double result = convert_double(csv_read(temp_path));
    result.resize(1 + 10 * N);
    if (!result.empty()) if (!result[0].empty()) if (result[0][0] == double(N)) check_temp = true;

    // リセット処理
    if (!check_temp) {
        result[0] = { double(N), 0 };
        remove(temp_progress_path.c_str());
    }
    
    // 計測開始
    chrono::system_clock::time_point all_start = chrono::system_clock::now();

    // 処理部
    vector<val_t> base;
    int cap = pow(10, N);
    int power = 1;
    if (check_temp) { power = result[0][1]; }
    while (power <= 10 * N) {
        // 設定
        int size = int(pow(10,power/10.0));
        bool check_temp2 = false;
        csv_double data = convert_double(csv_read(temp_progress_path));
        data.resize(1+M);
        if (!data.empty()) if (!data[0].empty()) if (data[0][0] == power) check_temp2 = true;
        if (!check_temp2) {
            data[0] = {double(power), 0};
            remove(temp_progress_path.c_str());
        }

        // 進捗バー1
        view_progress_bar({ "1. main_progress", { size, cap } }, false);
        
        // ループ
        while (data[0][1] < M) {
            // 進捗バー2
            view_progress_bar({ "2. loop_progress", { data[0][1] + 1, M } });

            // 設定
            vector<val_t> base = get_rand_vector(size,size);

            // 本処理
            vector<double> temp = sort_launch(base, 1023);

            // 後ろに渡す
            data[data[0][1]+1] = temp;

            // バックアップ用
            data[0][1]++;
            // csv_write(convert_string(data), temp_progress_path);
        }

        // 2行戻す
        fprintf(stderr, "\033[2A");

        // 平均値計算
        vector<double> list1(10);
        for (int i = 0; i < 10; i++) {
            vector<double> list2(M);
            for (int j = 0; j < M; j++) {
                list2[j] = data[j+1][i];
            }
            list1[i] = get_mean(list2);
        }
        result[power] = list1;

        // バックアップ用
        result[0][1] = power++;
        csv_write(convert_string(result), temp_path);
}

    // 計測終了
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
    csv_write(convert_string(result), "./result/amount.csv");

    // 削除
    remove(temp_path.c_str());
    remove(temp_progress_path.c_str());

    return 0;
}