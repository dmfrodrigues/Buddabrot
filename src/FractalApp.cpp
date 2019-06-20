#include <wx/wx.h>
#include "FractalFrame.h"
#include "Buddhabrot.h"

class FractalApp: public wxApp{
private:
    FractalFrame *fractalFrame;
    bb *f;
public:
    virtual bool OnInit(){
        wxInitAllImageHandlers();
        f = new bb(250000, 10000); //addPt, numIt
        fractalFrame = new FractalFrame(f);
        fractalFrame->Show(true);
        return true;
    }
    int OnExit(){
        if(f!=NULL) delete f;
    }
};

wxIMPLEMENT_APP(FractalApp);
