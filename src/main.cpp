#include "imconfig-SFML.h"
#include "imgui.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
int main()
{
    sf::RenderWindow window(sf::VideoMode({(unsigned int)1280, (unsigned int)600}), "SFML Works!");
    window.setFramerateLimit(60);
}