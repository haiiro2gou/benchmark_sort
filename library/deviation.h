#ifndef deviation_h
#define deviation_h

#include <cmath>
#include <vector>

double get_mean(std::vector<double> base) {
    double ret = 0;

    for (int i = 0; i < base.size(); i++) {
        ret += base[i];
    }
    ret /= 1.0 * base.size();

    return ret;
}

double get_variance(std::vector<double> base) {
    double mean = get_mean(base);

    double ret = 0;
    for (int i = 0; i < base.size(); i++) {
        ret += pow(base[i] - mean, 2);
    }
    ret /= 1.0 * base.size();

    return ret;
}

double get_deviation(std::vector<double> base) {
    double variance = get_variance(base);

    return sqrt(variance);
}

#endif