#include "Buddhabrot.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <thread>

///Constants
const bb::complex_t bb::w(6.0L);
const bb::ComplexNum bb::orig(bb::complex_t(-3.0L), bb::complex_t(-3.0L));

///Constructor
bb::bb(iter_t addPoint, iter_t numberIt):FractalBitmap(),px(*((wxBitmap*)this)),addPt(addPoint),NumIt(numberIt){}

bb::bb(const bb &p):FractalBitmap(),px(*((wxBitmap*)this)),addPt(p.addPt),NumIt(p.NumIt){
    std::lock_guard<std::mutex> lock(Mutex);

    Create(p.GetCenter(), p.GetStep(), p.GetSize());
    numPt = p.numPt;
    const size_t N = GetWidth()*GetHeight();
    std::copy(p.seeds, p.seeds+N, seeds);
    for(unsigned i = 0; i < NThreads; ++i) std::copy(p.z[i], p.z[i]+NumIt, z[i]);
    for(size_t i = 0; i < N; ++i) NUM[i] = (iter_t)p.NUM[i];
    std::copy(p.X    , p.X    +N, X    );
    
}

///Create
void bb::Create(ComplexNum o, complex_t st, wxSize s){
    std::lock_guard<std::mutex> lock(Mutex);

    FractalBitmap::Create(o, st, s);

    px = wxNativePixelData(*((wxBitmap*)this));

    const unsigned N = GetWidth()*GetHeight();

    numPt = 0;
    if(seeds!=NULL){ delete[] seeds; } seeds = new long[NThreads]; srand((unsigned)time(NULL)); std::generate(seeds, seeds+NThreads, rand);
    if(z    !=NULL){ for(unsigned i = 0; i < NThreads; ++i) delete[] z[i]; delete[] z; }
    z = new ComplexNum*[NThreads]; for(unsigned i = 0; i < NThreads; ++i) z[i] = new ComplexNum[NumIt];
    if(NUM  !=NULL){ delete[] NUM  ; } NUM = new std::atomic<iter_t>[N]; std::fill(NUM,NUM+N,0);
    if(X    !=NULL){ delete[] X    ; } X = new unsigned[N];
}

///Clone
bb* bb::Clone() const{
    return new bb(*this);
}

bb::~bb(){
    if(seeds!=NULL) delete[] seeds;
    if(z    !=NULL){ for(unsigned i = 0; i < NThreads; ++i) delete[] z[i]; delete[] z; }
    if(NUM  !=NULL) delete[] NUM;
    if(X    !=NULL) delete[] X;
}

void bb::Update(){
    std::lock_guard<std::mutex> lock(Mutex);

    std::thread *ArrThreads[NThreads];

    for(unsigned i = 0; i < NThreads; ++i){
        ArrThreads[i] = new std::thread(&bb::UpdateMathLim, this, i, addPt/NThreads);
    }
    for(unsigned i = 0; i < NThreads; ++i) ArrThreads[i]->join();
    for(unsigned i = 0; i < NThreads; ++i) delete ArrThreads[i];
    UpdatePixels();

    numPt += (addPt/NThreads)*NThreads;


}

void bb::UpdateMathLim(unsigned index, iter_t addPoint){
    srand((unsigned)seeds[index]);


    bool ESCAPED;
    ComplexNum c;
    ComplexNum *z_ = z[index];
    for(iter_t i = addPoint; --i;){
        ESCAPED = false;
        c = randComplex();
        if(isCardioid_isPeriod2Bulb(c)) continue;
        z_[0] = ComplexNum(complex_t(0.0L),complex_t(0.0L));
        iter_t nit;
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
            for(iter_t k = 1; k <= nit; ++k){
                u = z_[k]-GetOrigin();
                x = (long long)(u.real()/(+GetStep()));
                y = (long long)(u.imag()/(-GetStep()));
                if(0 <= x && x < GetWidth() && 0 <= y && y < GetHeight()) ++NUM[x+y*GetWidth()];
                u = ComplexNum(z_[k].real(), -z_[k].imag())-GetOrigin();
                x = (long long)(u.real()/(+GetStep()));
                y = (long long)(u.imag()/(-GetStep()));
                if(0 <= x && x < GetWidth() && 0 <= y && y < GetHeight()) ++NUM[x+y*GetWidth()];
            }
        }
    }
    seeds[index] = rand();
}

void bb::UpdatePixels(){
    wxNativePixelData::Iterator p(px);
    const unsigned N = GetWidth()*GetHeight();
    for(unsigned i = 0; i < N; ++i) X[i] = (unsigned)NUM[i];
    std::nth_element(X, X+N-200, X+N);
    iter_t m = X[N-200];

    for(unsigned i = 0; i < N; ++i){
        p.MoveTo(px, i%GetWidth(), i/GetWidth());



        unsigned x = (unsigned)std::min(bb::ColorT((255.0*(double)NUM[i])/(double)m),bb::ColorT(255)); ///continuous pattern, original formula



        p.Red()   = (unsigned char)x;
        p.Green() = (unsigned char)x;
        p.Blue()  = (unsigned char)x;
    }
}

bool bb::SaveFile(const wxString& name, wxBitmapType type, const wxPalette *palette) const{
    std::lock_guard<std::mutex> lock(Mutex);

    wxBitmap::SaveFile(name, type, palette);

    std::ofstream ostrm(name.ToStdString() + ".txt");

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Winline"
    ostrm << "timedate\t"    << wxDateTime::Now().Format("%d-%b-%Y %H:%M:%S").c_str() << "\n"
          << "timeelapsed\t" << std::setprecision( 8) << 0.0                          << "\n"
          << "re(c)\t"       << std::setprecision(20) << GetCenter().real()           << "\n"
          << "im(c)\t"       << std::setprecision(20) << GetCenter().imag()           << "\n"
          << "step\t"        << std::setprecision(20) << GetStep()                    << "\n"
          << "size.x\t"      << GetWidth()                                            << "\n"
          << "size.y\t"      << GetHeight()                                           << "\n"
          << "NumPt\t"       << numPt                                                 << "\n" << std::flush;
    #pragma GCC diagnostic pop
    
    ostrm.close();
    return true;
}

bb::ComplexNum bb::randComplex(){
    unsigned long x = rand();
    unsigned long y = rand();
    bb::complex_t x_ = bb::complex_t(x)/bb::complex_t(RAND_MAX);
    bb::complex_t y_ = bb::complex_t(y)/bb::complex_t(RAND_MAX);
    return orig + bb::ComplexNum(x_*w,y_*w);
}
