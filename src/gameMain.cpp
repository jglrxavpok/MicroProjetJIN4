#include "gameMain.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

using namespace std;

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

int gameMain()
{
#ifdef WSL
    // uniquement pour que CLion se plaigne pas quand je lance le programme avec WSL
    // si vous n'êtes pas résolu à faire fonctionner Valgrind sous Windows, cette ligne ne vous servira pas
    setenv("DISPLAY", "127.0.0.1:0", true);
#endif
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Micro Projet JIN4");

    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = "resources/imgui.ini";

    sf::Clock deltaClock;
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        // TODO: rendu ImGui

        window.clear(sf::Color::White);

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}
