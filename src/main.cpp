#include "imconfig-SFML.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({(unsigned int) 1280, (unsigned int) 600}), "SFML Works!");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
    {
        return -1;
    }
    sf ::Clock deltaClock;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Update ImGui
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::SFML::Render(window);
        window.display();
    }
}