#include "BWin.h"

#include <windows.h>

#include <iostream>

///================================================================
///ROUTINES
string int2str(int a, int num_digits) {
    string ret; stringstream ss; ss << a; ss >> ret;
    num_digits -= ret.size(); while(num_digits) ret = "0" + ret;
    return ret;
}
template<class T> string print_num(string frmt, T obj){
    char buffer[100];
    sprintf(buffer, &frmt[0], obj);
    return string(buffer);
}
int getButtonPressed(const sf::Mouse& mouse){
    if(mouse.isButtonPressed(sf::Mouse::Left))   return 1;
    if(mouse.isButtonPressed(sf::Mouse::Middle)) return 2;
    if(mouse.isButtonPressed(sf::Mouse::Right))  return 3;
    return 0;
}
///================================================================
///BUDDHAWINDOW

void BuddhaWindow::createTexts(){
    if(!this->font.loadFromFile("C:/Windows/Fonts/consola.ttf")){MessageBox(NULL, "Font not loaded.", "Error", MB_ICONERROR); return;}
    this->text1.setFont(font); text1.setCharacterSize(text_character_size); text1.setFillColor(sf::Color::Black);
    this->text2.setFont(font); text2.setCharacterSize(text_character_size); text2.setFillColor(sf::Color::Black);
}
void BuddhaWindow::updateTexts(sf::Vector2i mouse_xy){
    cmplex origin = fractal.getOrigin();
    ld step = fractal.getStep();
    ld zoom = fractal.getZoom();
    ld width = fractal.getWidth();
    ulli totalNumPts = fractal.getTotalNumPts();
    ulli divider = fractal.getDividers().second;
    float time_interval = fractal.getTimeInterval();
    this->text1.setString(print_num("Re(c) = %+.19LE; ", origin.r+(ld)mouse_xy.x*step)+print_num("Zoom: %.0Lf;\n",      zoom)+
                          print_num("Im(c) = %+.19LE; ", origin.i-(ld)mouse_xy.y*step)+print_num("Horizontal diameter of the image: %LG", width));
    this->text2.setString(print_num("Time per update: %.7f s\n", time_interval)+
                          print_num("Number of points: %.4e; ", float(totalNumPts))+print_num("99.9th percentile: %ld", divider));
}
void BuddhaWindow::createRectangle(){
    this->rectangle.setFillColor(sf::Color::White);
    this->rectangle.setOutlineThickness(1);
    this->rectangle.setOutlineColor(sf::Color::Black);
}

void BuddhaWindow::updateTextPos(){
    sf::Vector2u sz = this->getSize();
    this->rectangle.setPosition(0, sz.y-H_rectangle);
    this->rectangle.setSize(sf::Vector2f(sz.x, H_rectangle));
    text1.setPosition(int(10), int(sz.y-H_rectangle+4));
    text2.setPosition(int(sz.x-400), int(sz.y-H_rectangle+4));
}

void BuddhaWindow::changedFullscreen(){
    this->setKeyRepeatEnabled(false);
    this->setMouseCursorVisible(false);
    this->updateTextPos();
}
void BuddhaWindow::inFullscreen(){
    this->create(sf::VideoMode::getFullscreenModes().front() , "Buddhabrot", sf::Style::Fullscreen);
    this->changedFullscreen();
}
void BuddhaWindow::outFullscreen(){
    this->create(sf::VideoMode(sz_initial_x, sz_initial_y), "Buddhabrot");
    this->changedFullscreen();
}

void BuddhaWindow::drawAll(){
    sf::Vector2u sz = fractal.getSize();

    this->clear();
    sf::Texture texture;
    sf::Sprite sprite;
    texture.create(sz.x, sz.y);
    texture.update(&fractal.getPixelsRef().front());
    sprite.setTexture(texture);
    this->draw(sprite);

    this->draw(this->rectangle);
    this->draw(this->text1);
    this->draw(this->text2);

    sf::Vector2i mouse_xy = mouse.getPosition(*this);

    sf::Vertex Vline[2] = {sf::Vector2f(mouse_xy.x, 0), sf::Vector2f(mouse_xy.x, sz.y)};
    sf::Vertex Hline[2] = {sf::Vector2f(0, mouse_xy.y), sf::Vector2f(sz.x, mouse_xy.y)};
    this->draw(Vline, 2, sf::Lines);
    this->draw(Hline, 2, sf::Lines);

    if (this->hasFocus() && (0 <= mouse_xy.x && mouse_xy.x < (int)sz.x && 0 <= mouse_xy.y && mouse_xy.y < (int)sz.y)){
        this->updateTexts(mouse_xy);
    }else{
        this->text1.setString(""); this->text2.setString("");
    }
}

BuddhaWindow::BuddhaWindow():
  sf::RenderWindow(sf::VideoMode(sz_initial_x, sz_initial_y), "Buddhabrot", sf::Style::Close),
  fractal(sf::Vector2u(sz_initial_x, sz_initial_y-H_rectangle), cmplex(0.0L, 0.0L), 1.0L),
  rectangle(sf::Vector2f(sz_initial_x, H_rectangle)){
    this->setPosition(sf::Vector2i(10,10));
    this->setKeyRepeatEnabled(false);
    this->setMouseCursorVisible(false);
    this->createTexts();
    this->createRectangle();
}

void BuddhaWindow::reset(){
    fractal.reset();
    this->updateTextPos();
}
void BuddhaWindow::update(){
    fractal.update();
    if(flag_fullscreen_prev != flag_fullscreen){
        if(flag_fullscreen) this->inFullscreen();
        else                this->outFullscreen();
        flag_fullscreen_prev = flag_fullscreen;
    }
    this->drawAll();
    this->display();
}

void BuddhaWindow::processEvents(){
    if(!this->isOpen()) return;
    while(this->pollEvent(this->event)){
        if(event.type == sf::Event::Closed){
            this->close();
        }else if(event.type == sf::Event::MouseWheelScrolled){
            ld mouse_x = event.mouseWheelScroll.x;
            ld mouse_y = event.mouseWheelScroll.y;
//            f::Vector2i mouse_xy = mouse.getPosition(*this);
            cmplex prev_origin = fractal.getOrigin();
            ld step = fractal.getStep();
            fractal.setZoom(fractal.getZoom()*pow(2.0L, (ld)event.mouseWheelScroll.delta));
            fractal.setCenter(cmplex(prev_origin.r + step*mouse_x,
                                     prev_origin.i - step*mouse_y));
            fractal.reset();
        }
    }


}

void BuddhaWindow::savePrintscreen(const vector<sf::Uint8>& pixels, sf::Vector2u sz){
    //Checking if the folder "Buddhabrot plotter - prinscreens" exists. If not, create it.
    string folder = ".\\Buddhabrot plotter - prinscreens"; CreateDirectory(&folder[0], NULL);
    //Checking for an available name to the new file
    int i = 0;
    string file_str = "Image-" + int2str(i, 4) + ".bmp";
    string filepath_str = folder + "\\" + file_str;
    while (ifstream(&filepath_str[0])) {
        if(++i > 9999) {
            MessageBox(NULL, "Printscreen naming error - more than 9999 files.", "Error", MB_ICONERROR);
            return;
        }
        file_str = "Image-" + int2str(i, 4) + ".bmp";
        filepath_str = folder + "\\" + file_str;
    }
    //Creating the new file
    ofstream print_screen; print_screen.open(&filepath_str[0]); print_screen.close();
    //Loading the pixels vector to a sf::Image and saving it to the file
    sf::Image image; image.create(sz.x, sz.y, &pixels.front());
    image.saveToFile(filepath_str);
    MessageBox(NULL, "Printscreen saved.", "Printscreen saved", MB_ICONINFORMATION); //SAVED AT???
}

