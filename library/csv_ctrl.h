#ifndef csv_ctrl_h
#define csv_ctrl_h

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>

typedef std::vector<std::vector<std::string>> csv;
typedef std::vector<std::vector<double>> csv_double;

csv convert_string(csv_double base) {
    csv res(base.size());
    for (int i = 0; i < base.size(); i++) {
        for (auto x : base[i]) {
            char str[20];
            std::sprintf(str, "%.9lf", x);
            res[i].push_back(str);
        }
    }
    return res;
}

csv_double convert_double(csv base) {
    csv_double res(base.size());
    for (int i = 0; i < base.size(); i++) {
        for (auto x : base[i]) {
            res[i].push_back(stod(x));
        }
    }
    return res;
}

std::vector<std::string> split(std::string &input, char delimiter) {
    std::istringstream stream(input);
    std::string field;
    std::vector<std::string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

csv csv_read(std::string path) {
    std::ifstream ifs(path);
    bool exists = true;
    if (!ifs) {
        exists = false;
        int pos = path.rfind("/");
        if (pos != std::string::npos) {
            std::string root = path.substr(0,pos);
            if (std::filesystem::create_directories(root.c_str()) == 0) exists = true;
        }
    }

    if (exists) {
        csv result;
        std::string line;

        while (getline(ifs, line)) {
            std::vector<std::string> strvec = split(line, ',');

            result.push_back(strvec);
        }

        return result;
    }

    return {{}};
}

void csv_write (csv input, std::string path, bool append = false) {
    std::ofstream ofs(path);
    bool exists = true;
    if (!ofs) {
        exists = false;
        int pos = path.rfind("/");
        if (pos != std::string::npos) {
            std::string root = path.substr(0,pos);
            if (std::filesystem::create_directories(root.c_str()) == 0) exists = true;
        }
    }

    if (exists) {
        for (auto part : input) {
            for (int i = 0; i < part.size(); i++) {
                if (append) {
                    std::fstream append(path,std::ios_base::app);
                }
                else {
                    ofs << part[i] << (i < part.size()-1 ? ',' : '\n');
                }
            }
        }
    }
    ofs.close();
}

#endif