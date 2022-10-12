#ifndef squick_sort_cpp
#define squick_sort_cpp

#include <list>
#include <functional>

template <class T, class C>
void squick_sort(std::list<T>& s, C cmp)
{
        if (s.empty()) return;
        std::list<T> a, b;
        const T pivot = s.front(); s.pop_front();
        while (!s.empty()) {
                const T& x = s.front();
                if (cmp(x, pivot))
                        a.splice(a.end(), s, s.begin());
                else
                        b.splice(b.end(), s, s.begin());
        }
        squick_sort(a, cmp); squick_sort(b, cmp);
        s.splice(s.end(), a); s.push_back(pivot); s.splice(s.end(), b);
}

template <class T>
inline void squick_sort(std::list<T>& s) { squick_sort(s, std::less<T>()); }

#endif // squick_sort_cpp