//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Scene.h"
#include "RNG.h"
#include "MusicLine.h"
#include "GameState.h"

class Game {
private:
    std::unique_ptr<Scene> scene = nullptr;
    float time = 0.0f;
    int mousePosX;
    int mousePosY;
    bool buttonPressed[sf::Mouse::ButtonCount] = {false};
    sf::RenderWindow& renderTarget;
    RNG rng{};
    GameState state{10.0f};

    std::shared_ptr<sf::Texture> badGuyTexture = nullptr;
    std::shared_ptr<sf::Texture> singleNotesTexture = nullptr;
    std::shared_ptr<sf::Texture> doubleNotesTexture = nullptr;

    /// la ligne de musique en train d'être dessinée (null si le clic gauche n'est pas enfoncé)
    std::shared_ptr<MusicLine> currentMusicLine = nullptr;

    constexpr static float ENEMY_SPAWN_PERIOD = 1.0f; // en secondes

    void spawnEnemy();
    void renderHealthBar();

public:
    /// 60Hz
    constexpr static float TARGET_UPDATE_PERIOD = 1.0/60.0;

    explicit Game(sf::RenderWindow& window);

    /// Appelé toutes les TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
    void update();

    /// Appelé autant que possible pour afficher le jeu
    /// @param renderTarget fenêtre sur laquelle on dessine
    /// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
    /// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
    void render(sf::RenderWindow& renderTarget, float partialTick);

    /// Appelé lorsque l'utilisateur ferme la fenêtre
    void shutdown();

    /// Appelé au début d'un clic
    void mousePressed(int x, int y, sf::Mouse::Button button);

    /// Appelé à la fin d'un clic
    void mouseReleased(int x, int y, sf::Mouse::Button button);

    /// Appelé lorsque la souris est déplacée
    void mouseMoved(int x, int y);

    void updateMousePos(int x, int y);

    std::shared_ptr<sf::Texture> loadTexture(std::string path);
};
