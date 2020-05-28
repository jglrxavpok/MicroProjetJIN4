#include "gameMain.h"
#include "Game.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <tmxlite/Map.hpp>
#include <box2d/b2_body.h>

using namespace std;

// TODO: configuration pour la résolution
constexpr int WIDTH = 1600;
constexpr int HEIGHT = 900;

/// 60Hz
constexpr float TARGET_UPDATE_PERIOD = 1.0/60.0;
/// On essaie pas de rattrapper plus de 10 frames
constexpr int MAX_CATCHUP_ATTEMPTS = 10;

void processEvents(sf::RenderWindow& window);

int gameMain()
{
#ifdef WSL
    // uniquement pour que CLion se plaigne pas quand je lance le programme avec WSL
    // si vous n'êtes pas résolu à faire fonctionner Valgrind sous Windows, cette ligne ne vous servira pas
    setenv("DISPLAY", "127.0.0.1:0", true);
#endif
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Micro Projet JIN4", sf::Style::Close | sf::Style::Titlebar);

    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = "resources/imgui.ini";

    sf::Clock deltaClock;
    float lag = 0.0; // in ms

    while(window.isOpen()) {
        sf::Time dt = deltaClock.restart();
        processEvents(window);

        lag += dt.asSeconds();

        int catchupAttempts = 0;
        while (lag >= TARGET_UPDATE_PERIOD && catchupAttempts < MAX_CATCHUP_ATTEMPTS) {
            update();
            lag -= TARGET_UPDATE_PERIOD;
            catchupAttempts++;
        }

        ImGui::SFML::Update(window, dt);
        // TODO: rendu ImGui

        render(window, lag / TARGET_UPDATE_PERIOD);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}

void processEvents(sf::RenderWindow& window) {
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
}
