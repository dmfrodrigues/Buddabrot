#include "Buddhabrot.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <thread>

///Constants
const bb::ComplexT bb::w(6.0L);
const bb::ComplexNum bb::orig(bb::ComplexT(-3.0L), bb::ComplexT(-3.0L));
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
    const unsigned N = GetSize().x*GetSize().y;
    if(seeds!=NULL) delete[] seeds; seeds = new long[NThreads]; srand(time(NULL)); std::generate(seeds, seeds+NThreads, rand);
    if(z    !=NULL){ for(unsigned i = 0; i < NThreads; ++i) delete[] z[i]; delete[] z; }
    z = new ComplexNum*[NThreads]; for(unsigned i = 0; i < NThreads; ++i) z[i] = new ComplexNum[NumIt];
    if(NUM  !=NULL) delete[] NUM  ; NUM = new std::atomic<IterationT>[N]; std::fill(NUM,NUM+N,0);
    if(X    !=NULL) delete[] X    ; X = new unsigned[N];

    px = wxNativePixelData(*((wxBitmap*)this));















}

///CreateNew
bb* bb::CreateNew(ComplexNum o, ComplexT st, wxSize s, bool IsCenter) const{
    bb *ret = new bb(addPt, NumIt);
    ret->New(o,st,s,IsCenter);
    return ret;
}

bb::~bb(){
    if(seeds!=NULL) delete[] seeds;
    if(z    !=NULL){ for(unsigned i = 0; i < NThreads; ++i) delete[] z[i]; delete[] z; }
    if(NUM  !=NULL) delete[] NUM;
    if(X    !=NULL) delete[] X;
}

void bb::UpdateMath(){
    std::thread *ArrThreads[NThreads];


    for(unsigned i = 0; i < NThreads; ++i){
        ArrThreads[i] = new std::thread(&bb::UpdateMathLim, this, i, addPt/NThreads);
    }
    for(unsigned i = 0; i < NThreads; ++i) ArrThreads[i]->join();
    for(unsigned i = 0; i < NThreads; ++i) delete ArrThreads[i];
    UpdatePixels();

    numPt += (addPt/NThreads)*NThreads;


}

void bb::UpdateMathLim(unsigned index, IterationT addPt){
    srand(seeds[index]);


    bool ESCAPED;
    ComplexNum c;
    ComplexNum *z_ = z[index];
    for(IterationT i = addPt; --i;){
        ESCAPED = false;
        c = randComplex();
        if(isCardioid_isPeriod2Bulb(c)) continue;
        z_[0] = ComplexNum(ComplexT(0.0L),ComplexT(0.0L));
        IterationT nit;
        for(nit = 1; nit < NumIt; ++nit){
            z_[nit] = z_[nit-1]*z_[nit-1]+c;
            if(std::norm(z_[nit]) > bailout_sqr){

                ESCAPED = true;


                break;
            }
        }
        if(ESCAPED){
            ComplexNum u;
            long long x, y;
            for(IterationT k = 1; k <= nit; ++k){
                u = z_[k]-origin; x = u.real()/(+step); y = u.imag()/(-step);
                if(0 <= x && x < GetSize().x && 0 <= y && y < GetSize().y) ++NUM[x+y*GetSize().x];
                u = ComplexNum(z_[k].real(), -z_[k].imag())-origin; x = u.real()/(+step); y = u.imag()/(-step);
                if(0 <= x && x < GetSize().x && 0 <= y && y < GetSize().y) ++NUM[x+y*GetSize().x];
            }
        }
    }
    seeds[index] = rand();
}

void bb::UpdatePixels(){
    wxNativePixelData::Iterator p(px);
    const unsigned N = GetSize().x*GetSize().y;
    for(unsigned i = 0; i < N; ++i) X[i] = NUM[i];
    std::nth_element(X, X+N-200, X+N);
    IterationT m = X[N-200];

    for(unsigned i = 0; i < N; ++i){
        p.MoveTo(px, i%GetSize().x, i/GetSize().x);



        unsigned x = std::min(bb::ColorT(255.0*NUM[i]/m),bb::ColorT(255)); ///continuous pattern, original formula



        p.Red()   = x;
        p.Green() = x;
        p.Blue()  = x;
    }
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

bb::ComplexNum bb::randComplex(){
    unsigned long x = rand();
    unsigned long y = rand();
    bb::ComplexT x_ = bb::ComplexT(x)/bb::ComplexT(RAND_MAX);
    bb::ComplexT y_ = bb::ComplexT(y)/bb::ComplexT(RAND_MAX);
    return orig + bb::ComplexNum(x_*w,y_*w);
}
