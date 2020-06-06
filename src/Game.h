//
// Created by jglrxavpok&gwenser
//

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Scene.h"
#include "RNG.h"
#include "boat_segment/MusicLine.h"
#include "boat_segment/GameState.h"
#include "GameplaySegment.h"


class Game {
private:
    int mousePosX;
    int mousePosY;
    bool buttonPressed[sf::Mouse::ButtonCount] = {false};
    sf::RenderWindow& renderTarget;
    unique_ptr<GameplaySegment> currentSegment;

    void updateMousePos(int x, int y);

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

    void keyPressed(sf::Event::KeyEvent event);

    void keyReleased(sf::Event::KeyEvent event);

    ///Pour charger les buffer pour les sons
    static std::unique_ptr<sf::SoundBuffer> loadBuffer(std::string path);

    ///Pour charger les sons
    static std::shared_ptr<sf::Sound> loadSound(std::unique_ptr<sf::SoundBuffer>& buffer);

    ///Pour charger des images
    static std::shared_ptr<sf::Texture> loadTexture(std::string path);

    sf::RenderWindow& getRenderTarget();

    void setGameplay(unique_ptr<GameplaySegment> gameplay);
};