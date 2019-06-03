#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include "FractalBitmap.h"

#include <deque>
#include <vector>
#include <list>

class bb: public FractalBitmap{
private:
    ///PRIVATE TYPEDEFS
    typedef float ColorT;

    ///PRIVATE STATIC CONSTANTS
    static const ComplexT bailout;
    static const ComplexT bailout_sqr;

    ///MEMBER VARIABLES
    ComplexT            step;   ///Difference between consecutive pixels
    ComplexNum          origin; ///Upper-left corner
    ComplexNum          center; ///Center of the fractal
    IterationT          numPt=0;///Total number of iterations performed over the fractal
    const IterationT    addPt;
    const IterationT    NumIt;
    ComplexNum          *z;
    IterationT          *NUM=NULL;    ///Number of iterations
    wxNativePixelData   px;     ///PixelData, to access bmp

    ///PRIVATE FUNCTIONS
    /**
     * Update pixels in [L,R) by making an additional addIt iterations
     */
    void UpdateMathLim(IterationT addPt);
    /**
     * Update pixels whose indexes are in q, based on the information in the variables
     */
    void UpdatePixels();

    ///PRIVATE STATIC FUNCTIONS
    static inline bool isCardioid_isPeriod2Bulb(const ComplexNum& c){
        ComplexT q = (c.real()-ComplexT(0.25L))*(c.real()-ComplexT(0.25L)) + c.imag()*c.imag();    ///isCardioid
        return (ComplexT(4.0L)*q*(q+(c.real()-ComplexT(0.25L))) < c.imag()*c.imag())||       ///isCardioid
               ((c.real()+ComplexT(1.0L))*(c.real()+ComplexT(1.0L)) + c.imag()*c.imag() < ComplexT(0.0625L)); ///isPeriod2Bulb
    }

public:
    /**
     * Constructor
     */
    bb(IterationT addPoint, IterationT numberIt);
    /**
     * New
     */
    void New(ComplexNum o, ComplexT st, wxSize s, bool IsCenter = false);
    /**
     * CreateNew
     */
    bb* CreateNew(ComplexNum o, ComplexT st, wxSize s, bool IsCenter = false);
    /**
     * Destructor
     */
    ~bb();

    ///CALCULATIONS ==================================================
    /**
     * Update all pixels by making an additional addIt iterations
     */
    void UpdateMath();

    ///GET FUNCTION ==================================================
    ComplexNum GetOrigin()         const{ return origin; }
    ComplexNum GetCenter()         const{ return center; }
    ComplexT   GetStep()           const{ return step;   }
    IterationT GetNum()            const{ return numPt;  }
    IterationT GetTimeUnit()       const{ return addPt;  };
    ComplexT   GetHorizontalSize() const{ return step*(ComplexT)GetSize().x; }

    ///OTHER UTILITIES
    bool SaveFile(const wxString& name, wxBitmapType type, const wxPalette *palette = NULL) const;
};

#endif
