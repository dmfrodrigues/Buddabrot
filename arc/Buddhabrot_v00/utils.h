#ifndef CPP_UTILS
#define CPP_UTILS

#include <vector>
#include <algorithm>

template<class T> std::pair<T,T> prctile(std::vector<T> vtr, long double p0, long double p1){
    std::sort(vtr.begin(), vtr.end());
    return std::pair<T,T>(vtr[int(p0 * (long double)(vtr.size()-1))],
                          vtr[int(p1 * (long double)(vtr.size()-1))]);
}
template<class T> T rand_num(T l, T r){
    return l + T(rand())*(r-l)/T(RAND_MAX);
}

#endif // CPP_UTILS
