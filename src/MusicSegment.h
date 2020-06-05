//
// Created by gwenser
//

#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "Sound.h"
#include "GameplaySegment.h"
#include "Game.h"

#define TEMPO 70

class MusicSegment: public GameplaySegment {
public:
    ///Pour rien afficher sur le clavier utiliser cette texture pour le sprite "keySprite"
    std::shared_ptr<sf::Texture> noKeyImage = Game::loadTexture("resources/no_key.png");

private:
    std::vector<std::string> names = { "A", "B", "C", "D", "E", "F", "G" };

    ///Musique 1
    std::vector<std::string> music{ "G", "A", "B", "G", "G", "F", "E", "C", "D", "E", "F", "E", "F", "G", "G", "F", "E", "D", "E", "F", "D", "E", "C", "E", "G", "G", "A", "B", "G", "G", "F", "E", "C", "D", "E", "F", "E", "F", "G", "G", "F", "E", "D", "E", "F", "D", "E", "C", "E", "G" };

    ///Image de fond du clavier dans sa globalité
    std::shared_ptr<sf::Texture> keyboardImage;

    std::map<std::string, std::shared_ptr<Sound>> notes;

    ///Dernière image de note utilisée
    std::shared_ptr<Sound> lastPressKey;

    sf::Sprite keySprite;
    sf::Sprite keyBoardSprite;
    int notePlay = 0;
    int ticks = 0;
    int lives = 3;
    int gameOver = 0;
    int playerPlayed = 1;

public:
    ///initialisation chargement des textures
    explicit MusicSegment(Game& game);

    ///Joue la note appuyée
    void playSound(std::string note);

    ///regarde si la note jouée est la bonne, met lastPressKey à cette note et set state de note
    void checkNote(std::string note);


    /// Appelé toutes les TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
    void update() override;

    void playMusic();

    /// Appelé autant que possible pour afficher le jeu
    /// @param renderTarget fenêtre sur laquelle on dessine
    /// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
    /// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
    void render(sf::RenderWindow& renderTarget, float partialTick) override;

    void keyPressed(sf::Event::KeyEvent event) override;

    void shutdown() override;
};
