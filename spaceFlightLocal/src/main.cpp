#include <iostream>
#include"game.h"
#include<time.h>

int main(){
    start : 
{
    srand(time(0));

    Game gm;

    while (gm.isRunning())
    {
        gm.input();

        gm.update();

        gm.render();
    }
    
    std::cin.get();
}
    goto start;
    
    return 0;
}