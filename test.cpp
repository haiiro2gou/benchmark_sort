#include "library/csv_ctrl.h"

using namespace std;

int main() {
    std::string path = "./temp/example.csv";
    csv example(10,vector<double>(10));
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            example[i][j] = i + j;
        }
    }
    csv_write(example, path);
    printf("Done!\n");

    return 0;
}