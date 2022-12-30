#ifndef med3_h
#define med3_h

#include <functional>
#include <iterator>

template <class T, class C>
inline T med3(const T& a, const T& b, const T& c, C cmp)
{
        return cmp(a, b) ?
                (cmp(b, c) ? b : cmp(c, a) ? a : c) :
                (cmp(c, b) ? b : cmp(a, c) ? a : c);
}

template <class T>
inline T med3(const T& a, const T& b, const T& c)
{
        return med3(a, b, c, std::less<T>());
}

#endif