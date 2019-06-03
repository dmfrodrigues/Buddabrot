#include <wx/wx.h>
#include "FractalFrame.h"
#include "Buddhabrot.h"

///MyApp will be our inherited wxApp
class FractalApp: public wxApp{
private:
    FractalFrame *fractalFrame;
    bb *f;
public:
    virtual bool OnInit(){
        wxInitAllImageHandlers();
        f = new bb(100000, 10000); //addPt, numIt
        fractalFrame = new FractalFrame(f);
        fractalFrame->Show(true);
        return true;
    }
};
///MACRO - "main" function
///creates an instance of MyApp and starts the program
wxIMPLEMENT_APP(FractalApp);
