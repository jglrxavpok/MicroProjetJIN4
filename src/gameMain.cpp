#include "gameMain.h"
#include "Game.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <tmxlite/Map.hpp>
#include <box2d/b2_body.h>
#include <iostream>
#include "BoatSegment.h"
#include "MusicSegment.h"

using namespace std;

// TODO: configuration pour la résolution
constexpr int WIDTH = 1600;
constexpr int HEIGHT = 900;

/// On essaie pas de rattrapper plus de 10 frames
constexpr int MAX_CATCHUP_ATTEMPTS = 10;

void processEvents(Game& game, sf::RenderWindow& window);

int gameMain()
{
#ifdef WSL
    // uniquement pour que CLion se plaigne pas quand je lance le programme avec WSL
    // si vous n'êtes pas résolu à faire fonctionner Valgrind sous Windows, cette ligne ne vous servira pas
    setenv("DISPLAY", "127.0.0.1:0", true);
#endif
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Micro Projet JIN4", sf::Style::Close);

    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = "resources/imgui.ini";

    sf::Clock deltaClock;
    float lag = 0.0; // in ms

    Game game{window};
    game.setGameplay(move(make_unique<MusicSegment>(game)));

    while(window.isOpen()) {
        sf::Time dt = deltaClock.restart();
        processEvents(game, window);

        lag += dt.asSeconds();

        int catchupAttempts = 0;
        while (lag >= Game::TARGET_UPDATE_PERIOD && catchupAttempts < MAX_CATCHUP_ATTEMPTS) {
            game.update();
            lag -= Game::TARGET_UPDATE_PERIOD;
            catchupAttempts++;
        }

        ImGui::SFML::Update(window, dt);
        // TODO: rendu ImGui
        game.render(window, lag / Game::TARGET_UPDATE_PERIOD);
        ImGui::SFML::Render(window);
        window.display();
    }
    game.shutdown();
    ImGui::SFML::Shutdown();

    return 0;
}

void processEvents(Game& game, sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed: {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                sf::Mouse::Button button = event.mouseButton.button;
                game.mousePressed(x, y, button);
                break;
            }

            case sf::Event::MouseButtonReleased: {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                sf::Mouse::Button button = event.mouseButton.button;
                game.mouseReleased(x, y, button);
                break;
            }

            case sf::Event::MouseMoved: {
                int x = event.mouseMove.x;
                int y = event.mouseMove.y;
                game.mouseMoved(x, y);
                break;
            }

            case sf::Event::KeyPressed:
                game.keyPressed(event.key);
                break;

            case sf::Event::KeyReleased:
                game.keyReleased(event.key);
                break;
        }
    }
}
