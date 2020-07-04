#ifndef BUDDHABROT_H_INCLUDED
#define BUDDHABROT_H_INCLUDED

#include "FractalBitmap.h"

#include <atomic>
#include <deque>
#include <vector>
#include <list>

class bb: public FractalBitmap{
private:
    ///PRIVATE TYPEDEFS
    typedef float ColorT;

    ///PRIVATE STATIC CONSTANTS
    static const complex_t w;
    static const ComplexNum orig;









    static constexpr complex_t bailout = 8.0L;
    static constexpr complex_t bailout_sqr = bailout*bailout;


    static constexpr unsigned NThreads = 4;

    ///MEMBER VARIABLES
    wxNativePixelData       px;     ///PixelData, to access bmp
    iter_t                  numPt=0;///Total number of iterations performed over the fractal
    long                    *seeds = NULL; //seeds for random numbers
    ComplexNum              **z = NULL;
    std::atomic<iter_t>     *NUM=NULL;    ///Number of iterations
    unsigned                *X=NULL;        //array to sort pixels
    const iter_t            addPt;
    const iter_t            NumIt;
    mutable std::mutex  Mutex;


    ///PRIVATE FUNCTIONS
    /**
     * Update pixels in [L,R) by making an additional addIt iterations
     */
    void UpdateMathLim(unsigned index, iter_t addPt);
    /**
     * Update pixels whose indexes are in q, based on the information in the variables
     */
    void UpdatePixels();





    static ComplexNum randComplex();

    ///PRIVATE STATIC FUNCTIONS
    static inline bool isCardioid_isPeriod2Bulb(const ComplexNum& c){
        complex_t q = (c.real()-complex_t(0.25L))*(c.real()-complex_t(0.25L)) + c.imag()*c.imag();    ///isCardioid
        return (complex_t(4.0L)*q*(q+(c.real()-complex_t(0.25L))) < c.imag()*c.imag())||       ///isCardioid
               ((c.real()+complex_t(1.0L))*(c.real()+complex_t(1.0L)) + c.imag()*c.imag() < complex_t(0.0625L)); ///isPeriod2Bulb
    }

public:
    /**
     * Constructor
     */
    bb(iter_t addPoint, iter_t numberIt);

    bb(const bb &p);

    void Create(ComplexNum o, complex_t st, wxSize s) override;

    bb* Clone() const override;

    ~bb();

    ///CALCULATIONS ==================================================
    /**
     * Update all pixels by making an additional addIt iterations
     */
    void Update();

    ///GET FUNCTION ==================================================
    iter_t GetNum()            const{ return numPt;  }
    iter_t GetCyclesPerRun()   const{ return addPt;  };


    ///OTHER UTILITIES
    bool SaveFile(const wxString& name, wxBitmapType type, const wxPalette *palette = NULL) const;
};

#endif
