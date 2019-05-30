#include "Mandelbrot.h"

#include <chrono>
#include <iostream>

typedef std::chrono::high_resolution_clock hrclock;

int main(){
    mb *f = new mb();
    f->New({-1.375L,0.0L}, 20.0L, wxSize(2000,1000), 2.5L, true);
    auto t1 = hrclock::now();
    for(int i = 0; i < 10; ++i){
        std::cout << f->GetNotEscaped() << std::endl;
        f->UpdateMath(200);
    }
    auto t2 = hrclock::now();
    auto dt = std::chrono::duration<long double>(t2-t1);
    std::cout << dt.count() << std::endl;
    return 0;
}
