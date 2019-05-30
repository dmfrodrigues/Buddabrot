#include <bits/stdc++.h>
#include "BWin.h"

using namespace std;

typedef BuddhaWindow BWin;

int main(){
    BWin win;
    win.reset();
    while(win.isOpen()){
        win.update();
        win.processEvents();
    }
}
