#include <wx/wx.h>
#include "FractalFrame.h"
#include "Mandelbrot.h"

///MyApp will be our inherited wxApp
class FractalApp: public wxApp{
private:
    FractalFrame *fractalFrame;
    mb *f;
public:
    virtual bool OnInit(){
        wxInitAllImageHandlers();
        f = new mb();
        fractalFrame = new FractalFrame(f);
        fractalFrame->Show(true);
        return true;
    }
};
///MACRO - "main" function
///creates an instance of MyApp and starts the program
wxIMPLEMENT_APP(FractalApp);
