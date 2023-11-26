//* benchmark_permutation.cpp
// ランダムなデータ列を生成し、処理時間の外れ値を出力します
// */

#include <iomanip>
#include <iostream>
#include <random>

#include "library/csv_ctrl.h"
#include "library/deviation.h"
#include "library/progress_bar.h"
#include "library/sort_launch.h"

int N, M;
void input() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    fprintf(stderr, "N> ");
    std::cin >> N;
    if (N > 65536) {
        N = 65536;
        fprintf(stderr, "N is too big, so N has been set to 65536.\n");
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
    string temp_path = "./temp/permutation/.csv";
    string temp_result_path = "./temp/permutation/result.csv";

    // csvの先頭から情報を取得していたら途中から
    bool check_temp = false;
    csv_double temp = convert_double(csv_read(temp_path));
    temp.resize(3);
    if (!temp.empty()) if (!temp[0].empty()) if (temp[0][0] == N) check_temp = true;

    // 計測開始
    chrono::system_clock::time_point all_start = chrono::system_clock::now();
    
    // 本処理
    vector<val_t> base(N);
    csv_double deviation_base;
    if (!check_temp) {
        temp[0] = {double(N), 0};
        remove(temp_result_path.c_str());
    }
    const int soft_cap = max(M / 10, 1000);
    int cnt = 0;
    bool init = true;
    bool init_during = true;
    fstream fs;
    while (temp[0][1] < M) {
        // データ生成
        random_device get_rand_dev;
        mt19937 get_rand_mt(get_rand_dev());
        for (int j = 0; j < N; j++) {
            base[j].key = j;
            base[j].data = j;
        }
        shuffle(base.begin(), base.end(), get_rand_mt);

        // ソート処理
        vector<double> data = sort_launch(base, 1023);

        // 平均値更新
        /*
        if (temp[1].empty()) {
            temp[1] = data;
        }
        else if (!init) for (int i = 0; i < 10; i++) {
            temp[1][i] = ((temp[1][i] * temp[0][1]) + data[i]) / (temp[0][1] + 1);
        }
        */

        if (init) {
            // 標準偏差初期化
            if (cnt >= soft_cap) {
                vector<double> deviation(10);
                temp[1].resize(10);
                for (int i = 0; i < 10; i++) {
                    vector<double> base(soft_cap);
                    for (int j = 0; j < soft_cap; j++) {
                        base[j] = deviation_base[j][i];
                    }
                    temp[1][i] = get_mean(base);
                    deviation[i] = get_deviation(base);
                }
                temp[2] = deviation;

                // フラグ折り
                init = false; 
            }
            else {
                deviation_base.push_back(data);
            }
        }
        else {
            // 外れ値チェック
            for (int i = 0; i < 10; i++) {
                if (abs(data[i] - temp[1][i]) > 10 * temp[2][i]) {
                    string sample = "0x";
                    stringstream ss;
                    for (int i = 0; i < N; i++) {
                        ss << setw(4) << setfill('0') << hex << base[i].key;
                    }
                    sample += ss.str();
                    string output = sample + ',' + to_string(i) + ',' + to_string(data[i]) + '\n';
                    fs.open(temp_result_path, ios_base::app);
                    if (fs.is_open()) {
                        fs.write(output.c_str(), output.length());
                    }
                    fs.close();
                }
            }
        }
        
        // ループ用
        if (cnt >= soft_cap) temp[0][1]++;
        else cnt++;

        // 進捗バー改
        view_progress_bar({ "1. init_progress", { cnt, soft_cap } }, false);
        view_progress_bar({ "2. main_progress", { int(temp[0][1]), M } });
        fprintf(stderr, "\r\033[2A");

        // バックアップ用書き込み
        if (int(temp[0][1]) % 100 == 1) csv_write(convert_string(temp), temp_path);
    }

    // 計測終了
    chrono::system_clock::time_point all_end = chrono::system_clock::now();
    int all_estimated = static_cast<int>(chrono::duration_cast<chrono::milliseconds>(all_end-all_start).count()/1000.0);
    fprintf(stderr,"\033[2Bfinish! (");
    if (all_estimated/(24*60*60)) fprintf(stderr, "%dd %02dh %02dm %02ds", all_estimated/(24*60*60), all_estimated/(60*60)%24, all_estimated/60%60, all_estimated%60);
    else if (all_estimated/(60*60)) fprintf(stderr, "%dh %02dm %02ds", all_estimated/(60*60)%24, all_estimated/60%60, all_estimated%60);
    else if (all_estimated/60) fprintf(stderr, "%dm %02ds", all_estimated/60%60, all_estimated%60);
    else if (all_estimated) fprintf(stderr, "%ds", all_estimated%60);
    else fprintf(stderr, "<1s");
    fprintf(stderr, ")\n");

    // 最終記録
    csv result = csv_read(temp_path);
    csv get_result = csv_read(temp_result_path);
    result.insert(result.end(), get_result.begin(), get_result.end());
    csv_write(result, "./result/permutation.csv");

    // 削除
    remove(temp_path.c_str());
    remove(temp_result_path.c_str());

    return 0;
}
