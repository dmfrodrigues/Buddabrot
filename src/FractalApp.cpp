#include "FractalFrame.h"
#include "Buddhabrot.h"
#include <iostream>

class FractalApp: public wxApp{
private:
    FractalFrame *fractalFrame;
    bb *f;
public:
    virtual bool OnInit(){
        std::cerr << "+FractalApp::OnInit" << std::endl;
        wxInitAllImageHandlers();
        f = new bb(250000, 10000); //addPt, numIt
        fractalFrame = new FractalFrame(f);
        fractalFrame->Show(true);
        std::cerr << "-FractalApp::OnInit" << std::endl;
        return true;
    }
    int OnExit(){
        std::cerr << "+FractalApp::OnExit" << std::endl;
        delete f;
        return wxApp::OnExit();
    }
};

wxIMPLEMENT_APP(FractalApp);
