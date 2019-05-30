#ifndef BUDDHAWINDOW
#define BUDDHAWINDOW
///LIBS
#include <bits/stdc++.h>
#include "BuddhaFractal.h"

using namespace std;

typedef long double ld;
typedef unsigned long long int ulli;

class BuddhaWindow: public sf::RenderWindow{
private:
    ///================================================================
    ///STATIC CONSTS
    //Technical
    static const ulli H_rectangle = 40;
    static const ulli sz_initial_x = 1850; //1850
    static const ulli sz_initial_y = 950; //950
    static const ulli text_character_size = 13;
    ///================================================================
    ///PRIVATE PROPERTIES
    //Algorithm
    Fractal fractal;
    ///================================================================
    ///TECHNICAL OBJECTS/FUNCTIONS
    //Text
    sf::Font font;
    sf::Text text1;
    sf::Text text2;
    void createTexts();
    void updateTextPos();
    void updateTexts(sf::Vector2i mouse_xy);
    //Rectangle
    sf::RectangleShape rectangle;
    void createRectangle();
    //Fullscreen
    bool flag_fullscreen = false, flag_fullscreen_prev = false;
    void changedFullscreen();
    //Interaction
    sf::Mouse mouse;
    sf::Event event;
    //Fullscreens
    void inFullscreen();
    void outFullscreen();
    //Appearance and drawing
    void drawAll();
public:
    ///CONSTRUCTOR
    BuddhaWindow();
    ///WINDOW FUNCTIONS
    //Technical
    void reset();
    void update();
    //Events
    void processEvents();
    //Printscreens
    void savePrintscreen(const vector<sf::Uint8>& pixels, sf::Vector2u sz);
};

#endif // BUDDHAWINDOW
