// FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Game.h"

using namespace sf;

int main()
{
    //Configuring and creation of game:
    Game game;

    
    //Game is open when the loop conditions are true, allows for the game to close
    while (game.running())
    {
        //Checking for updates
        game.update();

        //Rendering the game
        game.render();
    }
    return 0;
}

