#ifndef BUDDHAFRACTAL
#define BUDDHAFRACTAL
///LIBS
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

using namespace std;
typedef long double ld;
typedef unsigned long long int ulli;

///================================================================
///CMPLEX
struct cmplex{
    ld r;
    ld i;
    cmplex(){}
    cmplex(ld r_, ld i_){r=r_;i=i_;}
};
bool cardioid_OR_p2bulb(const cmplex& c);

///================================================================
///FRACTAL
class Fractal{
private:
    ///================================================================
    ///STATIC CONSTANTS
    static constexpr ld escape_condition  = 2.0;
    static constexpr ld escape_condition2 = escape_condition*escape_condition;
    static const ulli num_iterations = 10000; ///no. of iterations performed before updating the window.
    static const ulli num_points_per_update = 40000;
//    (5000,70000); (10000,40000)
    ///================================================================
    ///PRIVATE MEMBERS
    sf::Vector2u sz;
    ld zoom;
    ld step;
    ld width;
    cmplex center;
    cmplex origin;
    vector<sf::Uint8> pixels;
    vector<ulli> counter;
    pair<ulli, ulli> dividers;
    ulli totalNumPts = 0;
    float timeInterval;
    ///================================================================
    ///PRIVATE FUNCTIONS
    void updateCounter();
    void updatePixels();
public:
    ///================================================================
    ///CONSTRUCTOR
    Fractal(sf::Vector2u sz_, cmplex center_, ld zoom_);
    ///================================================================
    ///PUBLIC FUNCTIONS
    void reset();
    void update();
    ///================================================================
    ///GETS
    sf::Vector2u                getSize         ()const{return sz;            }
    ld                          getZoom         ()const{return zoom;          }
    ld                          getStep         ()const{return step;          }
    ld                          getWidth        ()const{return width;         }
    cmplex                      getOrigin       ()const{return origin;        }
    pair<ulli,ulli>             getDividers     ()const{return dividers;      }
    ld                          getTotalNumPts  ()const{return totalNumPts;   }
    ld                          getTimeInterval ()const{return timeInterval;  }
    const vector<sf::Uint8>&    getPixelsRef    ()const{return pixels;        }
    ///================================================================
    ///SETS
    void setZoom    (ld zoom_)          {this->zoom     = zoom_;    }
    void setCenter  (cmplex center_)    {this->center   = center_;  }
    void setSize    (sf::Vector2u sz_)  {this->sz       = sz_;      }
};

#endif // BUDDHAFRACTAL

