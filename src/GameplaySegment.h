//
// Created by jglrxavpok on 02/06/2020.
//

#pragma once
#include "Scene.h"
#include "RNG.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Game;

class GameplaySegment {
protected:
    std::unique_ptr<Scene> scene = nullptr;
    float time = 0.0f;
    sf::RenderWindow& renderTarget;
    RNG rng{};
    Game& game;

public:

    explicit GameplaySegment(Game& game);

    /// Appelé toutes les Game::TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
    virtual void update() = 0;

    /// Appelé autant que possible pour afficher le jeu
    /// @param renderTarget fenêtre sur laquelle on dessine
    /// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
    /// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
    virtual void render(sf::RenderWindow& renderTarget, float partialTick) = 0;

    /// Appelé lorsque l'utilisateur ferme la fenêtre
    virtual void shutdown() = 0;

    /// Appelé au début d'un clic
    virtual void mousePressed(int x, int y, sf::Mouse::Button button) {}

    /// Appelé à la fin d'un clic
    virtual void mouseReleased(int x, int y, sf::Mouse::Button button) {}

    /// Appelé lorsque la souris est déplacée
    virtual void mouseMoved(int x, int y) {}

    /// Appelé lorsque le joueur appuie sur une touche de son clavier
    virtual void keyPressed(sf::Event::KeyEvent) {};

    /// Appelé lorsque le joueur relâche une touche de son clavier
    virtual void keyReleased(sf::Event::KeyEvent) {};

    // Appelé lorsque la souris est déplacée avec un clic enfoncé (appelé après mouseMoved)
    virtual void mouseDrag(int x, int y, int dx, int dy, sf::Mouse::Button button) {}

};

