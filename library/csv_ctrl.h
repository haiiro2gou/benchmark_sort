#ifndef csv_ctrl_h
#define csv_ctrl_h

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>

typedef std::vector<std::vector<double>> csv;

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
            if (mkdir(root.c_str(), 0777) == 0 ) exists = true;
        }
    }

    if (exists) {
        csv result;
        std::string line;

        while (getline(ifs, line)) {
            std::vector<double> part;
            std::vector<std::string> strvec = split(line, ',');
            part.resize(strvec.size());

            for (int i = 0; i < strvec.size(); i++) {
                part[i] = stod(strvec.at(i));
            }

            result.push_back(part);
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
            if (mkdir(root.c_str(), 0777) == 0 ) exists = true;
        }
    }

    if (exists) {
        for (auto part : input) {
            for (int i = 0; i < part.size(); i++) {
                char str[20];
                sprintf(str, "%.9lf", part[i]);
                if (append) {
                    std::fstream append(path,std::ios_base::app);
                }
                else {
                    ofs << str << (i < part.size()-1 ? ',' : '\n');
                }
            }
        }
    }
}

#endif