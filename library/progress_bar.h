#ifndef progress_bar_h
#define progress_bar_h

#include <string>
#include <utility>

typedef std::pair<std::string, std::pair<int, int>> progress;
void view_progress_bar(progress bar, bool back = true) {
    std::string title = bar.first;
    int top = bar.second.first, bottom = bar.second.second;
    
    std::string progress = "";
    for (int i = 0; i < 30; i++) {
        if (top * 30 >= bottom * i) { progress += "#"; }
        else { progress += "."; }
    }

    fprintf(stderr, "\r\033[0K%s\n", title.c_str());
    fprintf(stderr, "\r\033[0K[%-30s] %6.2lf%% (%d/%d)\n", progress.c_str(), 100.0 * top / bottom, top, bottom);
    if (back) fprintf(stderr, "\r\033[2A");
}

#endif