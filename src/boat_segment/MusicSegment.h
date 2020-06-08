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
#define NBLIFE 3

class MusicSegment: public GameplaySegment {
public:
    ///Pour rien afficher sur le clavier utiliser cette texture pour le sprite "keySprite"
    std::shared_ptr<sf::Texture> noKeyImage = Game::loadTexture("resources/no_key.png");

private:
    std::vector<std::string> names = { "B", "A", "G", "F", "E", "D", "C" };

    ///Musique 1
    std::vector<std::string> music{ "G", "A", "B", "G", "G", "F", "E", "C", "D", "E", "F", "E", "F", "G", "G", "F", "E", "D", "E", "F", "D", "E", "C", "E", "G", "G", "A", "B", "G", "G", "F", "E", "C", "D", "E", "F", "E", "F", "G", "G", "F", "E", "D", "E", "F", "D", "E", "C", "E", "G" };

    ///Image de fond du clavier dans sa globalité
    std::shared_ptr<sf::Texture> keyboardImage;

    std::map<std::string, std::shared_ptr<Sound>> notes;

    ///Dernière image de note utilisée
    std::shared_ptr<Sound> lastPressKey;

    ///Texture des vies
    std::shared_ptr<sf::Texture> beatingHeart = Game::loadTexture("resources/beating_heart.png");
    std::shared_ptr<sf::Texture> heart = Game::loadTexture("resources/heart.png");
    std::shared_ptr<sf::Texture> deadHeart = Game::loadTexture("resources/dead_heart.png");

    // Texture de la clé de la partition
    std::shared_ptr<sf::Texture> musicKeyTexture = Game::loadTexture("resources/textures/music_key.png");

    ///Sprite pour les vies
    std::vector<unique_ptr<sf::Sprite>> livesSprites;


    sf::Sprite keySprite;
    sf::Sprite keyBoardSprite;
    int notePlay = 0;
    int ticks = 0;
    int lives = NBLIFE;
    int gameOver = 0;
    int playerPlayed = 1;
    int win = 0;
    int pourcentage = 0;
    sf::Font font;
    sf::Text pourcentagePrint;

    void hurtPlayer();
    void renderIncomingNotes();

public:
    ///initialisation chargement des textures
    explicit MusicSegment(Game& game);

    ///Joue la note appuyée
    void playSound(std::string note);

    ///regarde si la note jouée est la bonne, met lastPressKey à cette note et set state de note
    void checkNote(std::string note);


    /// Appelé toutes les TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
    void update() override;

    ///Suit la partition (music ici comme il n'y a qu'une seule partition)
    void playMusic();

    /// Appelé autant que possible pour afficher le jeu
    /// @param renderTarget fenêtre sur laquelle on dessine
    /// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
    /// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
    void render(sf::RenderWindow& renderTarget, float partialTick) override;

    void keyPressed(sf::Event::KeyEvent event) override;

    void shutdown() override;
};
