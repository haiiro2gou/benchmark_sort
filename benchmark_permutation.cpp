#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "library/csv_ctrl.h"
#include "library/sort_launch.h"

int N, M;
void input() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    fprintf(stderr, "N> ");
    std::cin >> N;
    if (N > 16) {
        N = 16;
        fprintf(stderr, "N is too big, so N has been set to 16.\n");
    }
    if (N < 1) {
        N = 1;
        fprintf(stderr, "N is too small, so N has been set to 1.\n");
    }
    fprintf(stderr, "M> ");
    std::cin >> M;
    if (M > 1e7) {
        fprintf(stderr, "N is too big, so the performance will be slow.\n");
    }
    if (M < 1e3) {
        M = 1000;
        fprintf(stderr, "M is too small, so M has been set to 1000.\n");
    }
}

int main() {
    using namespace std;
    // 設定部
    input();
    string temp_path = "./temp/permutation.csv";

    // 前処理
    vector<double> data;

    // csvの先頭から情報を取得していたら途中から
    bool check_temp = false;
    csv temp = csv_read(temp_path);
    if (!temp.empty()) if (!temp[0].empty()) if (temp[0][0] == N) check_temp = true;

    // 計測開始
    chrono::system_clock::time_point all_start = chrono::system_clock::now();
    
    // 本処理
    vector<val_t> base(N);
    int cur = 0;
    if (!check_temp) {
        remove(temp_path.c_str());
        csv id = {{double(N)}};
        csv_write(id, temp_path);
    }

    while (cur < M) {
        // データ生成
        random_device get_rand_dev;
        mt19937 get_rand_mt(get_rand_dev());
        for (int j = 0; j < N; j++) {
            base[j].key = j;
            base[j].data = j;
        }
        shuffle(base.begin(), base.end(), get_rand_mt);

        // ソート処理
        data = sort_launch(base);

        // データ記録
        csv output = csv_read(temp_path);

        // 外れ値チェック
        // 四分位偏差を使うことにする

        if (output.size() == 1) {
            output[0].emplace_back(cur);
            output.emplace_back(data);
        }
        else {
            int size = int(output[0][1]);
            for (int j = 0; j < 10; j++) {
                output[1][j] = ((output[1][j] * size) + data[j]) / (size + 1.0);
            }
            size++;
            output[0][1] = size;
        }
        csv_write(output, temp_path);

        // ループ用
        cur++;
    }

    // 計測終了
    chrono::system_clock::time_point all_end = chrono::system_clock::now();
    int all_estimated = static_cast<int>(chrono::duration_cast<chrono::milliseconds>(all_end-all_start).count()/1000.0);
    fprintf(stderr,"\033[Kfinish! (");
    if (all_estimated/(24*60*60)) fprintf(stderr, "%dd %02dh %02dm %02ds", all_estimated/(24*60*60), all_estimated/(60*60)%24, all_estimated/60%60, all_estimated%60);
    else if (all_estimated/(60*60)) fprintf(stderr, "%dh %02dm %02ds", all_estimated/(60*60)%24, all_estimated/60%60, all_estimated%60);
    else if (all_estimated/60) fprintf(stderr, "%dm %02ds", all_estimated/60%60, all_estimated%60);
    else if (all_estimated) fprintf(stderr, "%ds", all_estimated%60);
    else fprintf(stderr, "<1s");
    fprintf(stderr, ")\n");

    // 最終記録
    csv_write(csv_read(temp_path), "./result/permutation.csv");
    remove(temp_path.c_str());
    return 0;
}