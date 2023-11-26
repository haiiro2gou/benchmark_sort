#include <random>

#include "library/sort_launch.h"

using namespace std;

int main() {
    int N = 16;
    vector<val_t> base(N);
    random_device get_rand_dev;
    mt19937 get_rand_mt(get_rand_dev());
    for (int i = 0; i < N; i++) {
        base[i].key = i;
        base[i].data = to_string(i);
    }
    shuffle(base.begin(), base.end(), get_rand_mt);

    sort_launch(base, 1024);

    return 0;
}