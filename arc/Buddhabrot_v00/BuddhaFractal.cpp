#include "BuddhaFractal.h"
///LIBS
#include <windows.h>
//#include <CPPutils/utils.h>
#include "utils.h"

bool cardioid_OR_p2bulb(const cmplex& c) {
    double a = c.r-0.25L, b = c.i*c.i;
    double q = a*a + b;
    double d = c.r+1.0L;
    return (4.0L*q*(q+a) < b) || (d*d + c.i*c.i < 0.0625L);
}

Fractal::Fractal(sf::Vector2u sz_, cmplex center_, ld zoom_){
    sz = sz_;
    zoom = zoom_;
    center = center_;
    this->reset();
}

void Fractal::reset(){
    totalNumPts = 0;
    step = 2.5L/zoom/(ld)sz.y;
    width = step*ld(sz.x);
    origin = cmplex(center.r - step*ld(sz.x)/2.0L,
                    center.i + step*ld(sz.y)/2.0L);

    counter         = vector<ulli>   (sz.x*sz.y, 0);
    ulli size_pixels_vtr = sz.x*sz.y*4;
    pixels = vector<sf::Uint8>(size_pixels_vtr, 0);
    for(ulli i = 3; i < pixels.size(); i+=4){pixels[i] = 255;} ///set Alpha channel to 255 -> opaque colors
}

void Fractal::updateCounter(){
    vector<cmplex> vtr_z(num_iterations, cmplex(0.0L, 0.0L));
    int r, i;
    for(ulli index = 0; index < num_points_per_update; ++index){
        cmplex c(rand_num(origin.r, origin.r + ld(sz.x)*step),
                 rand_num(origin.i - ld(sz.y)*step, origin.i));

        if(!cardioid_OR_p2bulb(c)){
            cmplex z(0.0L, 0.0L), z2(0.0L, 0.0L);

            for (ulli iter = 0; iter < num_iterations; ++iter){
                vtr_z[iter].r = z2.r - z2.i + c.r;
                vtr_z[iter].i = 2*z.r*z.i + c.i;

                z = vtr_z[iter];
                z2.r = z.r*z.r;
                z2.i = z.i*z.i;

                if(z2.r + z2.i > escape_condition2){
                    if(iter != num_iterations){
                        for(ulli j = 0; j < iter; j++){
                            r = ((vtr_z[j].r - this->origin.r)/step);
                            i = ((vtr_z[j].i - this->origin.i)/(-step));
                            if((0 <= i) && (i < (int)sz.y) && (0 <= r) && (r < (int)sz.x)){ counter[i*sz.x + r]++; }
                            i = ((-vtr_z[j].i - this->origin.i)/(-step));
                            if((0 <= i) && (i < (int)sz.y) && (0 <= r) && (r < (int)sz.x)){ counter[i*sz.x + r]++; }
                        }
                    }
                    break;
                }
            }
        }
    }
    totalNumPts += num_points_per_update;
}

void Fractal::updatePixels(){
    pair<ulli,ulli> ans = prctile(this->counter, 0.500, 0.999); //0.999
    ulli min_divider = ans.first, max_divider = ans.second;
    if(max_divider == min_divider) ++max_divider;
    int color;
    for(ulli i = 0; i < this->counter.size(); ++i){
        color = this->counter[i]*255/(max_divider-min_divider) - min_divider*255/(max_divider-min_divider);
        color = min(color, 255);
        color = max(color,   0);
        this->pixels[4*i  ] =
        this->pixels[4*i+1] =
        this->pixels[4*i+2] = color;
    }
}

void Fractal::update(){
    ///Start timer ------------------
    LARGE_INTEGER frequency; QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER start0; QueryPerformanceCounter(&start0);
    ///------------------------------
    this->updateCounter();
    this->updatePixels();
    ///Get time ---------------------
    LARGE_INTEGER end0; QueryPerformanceCounter(&end0);
    timeInterval = (static_cast<float>(end0.QuadPart - start0.QuadPart) / frequency.QuadPart);
    ///------------------------------
}

