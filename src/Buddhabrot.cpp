#include "Buddhabrot.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <thread>

//reset && cd ../FractalApp/ && make && cd ../Buddhabrot/ && make -B && bin/main.exe > results.out

///Constants
const bb::ComplexT bb::bailout(8.0L); // 2.0 // 8.0
const bb::ComplexT bb::bailout_sqr = bb::bailout*bb::bailout;

///Constructor
bb::bb(IterationT addPoint, IterationT numberIt):addPt(addPoint),NumIt(numberIt),px(*((wxBitmap*)this)){}

///New
void bb::New(ComplexNum o, ComplexT st, wxSize s, bool IsCenter){
    Create(s, 24);
    step = st;
    origin = (IsCenter? GetOriginFromCenter(o, step, GetSize()) : o);
    center = GetCenterFromOrigin(origin, step, GetSize());
    numPt = 0;
    delete[] z; z = new ComplexNum[NumIt];
    const unsigned N = GetSize().x*GetSize().y;
    if(NUM) delete[] NUM; NUM = new IterationT[N]; std::fill(NUM,NUM+N,0);
    px = wxNativePixelData(*((wxBitmap*)this));
}

///CreateNew
bb* bb::CreateNew(ComplexNum o, ComplexT st, wxSize s, bool IsCenter){
    bb *ret = new bb(addPt, NumIt);
    ret->New(o,st,s,IsCenter);
    return ret;
}

bb::~bb(){
    delete[] z;
    delete[] NUM;
}
#include <iostream>
void bb::UpdateMath(){
    unsigned long long N = GetSize().x*GetSize().y;std::cout << "L41" << std::endl;
    UpdateMathLim(addPt);std::cout << "L42" << std::endl;
    UpdatePixels();
    numPt += addPt;
}

#include <limits>
#include <iostream>
bb::ComplexT w(6.0L);
bb::ComplexNum orig(bb::ComplexT(-3.0L), bb::ComplexT(-3.0L));
typedef unsigned long long ull;
bb::ComplexNum randComplex(){
    unsigned x = rand();
    unsigned y = rand();
    bb::ComplexT x_ = bb::ComplexT(x)/bb::ComplexT(RAND_MAX);
    bb::ComplexT y_ = bb::ComplexT(y)/bb::ComplexT(RAND_MAX);
    return orig + bb::ComplexNum(x_*w,y_*w);
}

void bb::UpdateMathLim(IterationT addPt){
    bool ESCAPED;
    ComplexNum c, u;
    long long x, y; std::cout << "L64" << std::endl;
    for(IterationT i = addPt; --i;){
        c = randComplex();
        if(isCardioid_isPeriod2Bulb(c)) continue;
        ESCAPED = false;
        z[0] = ComplexNum(ComplexT(0.0L),ComplexT(0.0L));
        IterationT j;
        for(j = 1; j < NumIt; ++j){
            z[j] = z[j-1]*z[j-1]+c;
            if(std::norm(z[j]) > bailout_sqr){
                ESCAPED = true;
                break;
            }

        }
        if(ESCAPED){
            for(IterationT k = 1; k <= j; ++k){
                u = z[k]-origin; x = u.real()/(+step); y = u.imag()/(-step);
                if(0 <= x && x < GetSize().x && 0 <= y && y < GetSize().y) ++NUM[x+y*GetSize().x];
                u = ComplexNum(z[k].real(), -z[k].imag())-origin; x = u.real()/(+step); y = u.imag()/(-step);
                if(0 <= x && x < GetSize().x && 0 <= y && y < GetSize().y) ++NUM[x+y*GetSize().x];
            }
        }
    } std::cout << "L87" << std::endl;

}

void bb::UpdatePixels(){
    const unsigned N = GetSize().x*GetSize().y;
    unsigned *X = new unsigned[N];
    for(unsigned i = 0; i < N; ++i) X[i] = NUM[i];
    std::sort(X,X+N);
    IterationT m = X[N-200];
    wxNativePixelData::Iterator p(px);
    for(unsigned i = 0; i < N; ++i){
        p.MoveTo(px, i%GetSize().x, i/GetSize().x);

        unsigned x = std::min(bb::ColorT(255.0*NUM[i]/m),bb::ColorT(255)); ///continuous pattern, original formula

        p.Red()   =
        p.Green() =
        p.Blue()  = x;
    }
    delete[] X;
}

bool bb::SaveFile(const wxString& name, wxBitmapType type, const wxPalette *palette) const{
    wxBitmap::SaveFile(name, type, palette);
    std::ofstream ostrm(name.ToStdString() + ".txt");
    ostrm << "timedate\t"    << wxDateTime::Now().Format("%d-%b-%Y %H:%M:%S").c_str() << "\n"
          << "timeelapsed\t" << std::setprecision( 8) << 0.0                          << "\n"
          << "re(c)\t"       << std::setprecision(20) << center.real()                << "\n"
          << "im(c)\t"       << std::setprecision(20) << center.imag()                << "\n"
          << "step\t"        << std::setprecision(20) << step                         << "\n"
          << "size.x\t"      << GetSize().x                                           << "\n"
          << "size.y\t"      << GetSize().y                                           << "\n"
          << "NumPt\t"       << numPt                                                 << "\n" << std::flush;
    ostrm.close();
    return true;
}
