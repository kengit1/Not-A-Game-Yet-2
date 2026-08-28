#include "Game.h"
#include <SFML/Graphics.hpp>

int main()
{
    Game g("build/config.txt");
    g.run();
    return 0;
}