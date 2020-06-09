//
// Created by jglrxavpok
//
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Scene.h"
#include "RNG.h"
#include "MusicLine.h"
#include "GameState.h"
#include "GameplaySegment.h"
#include "Game.h"
#include <tmxlite/Object.hpp>
#include <tmxlite/Layer.hpp>

class BoatSegment: public GameplaySegment {
private:
    GameState state{10.0f};

    std::shared_ptr<sf::Texture> badGuyTexture = nullptr;
    std::shared_ptr<sf::Texture> singleNotesTexture = nullptr;
    std::shared_ptr<sf::Texture> doubleNotesTexture = nullptr;

    /// la ligne de musique en train d'être dessinée (null si le clic gauche n'est pas enfoncé)
    std::shared_ptr<MusicLine> currentMusicLine = nullptr;

    /// son joué lorsque le joueur est blessé
    std::unique_ptr<sf::SoundBuffer> hurtSoundBuffer;

    /// son joué lorsque la ligne de musique est brisée
    std::unique_ptr<sf::SoundBuffer> lineBreakSoundBuffer;

    /// son joué lorsque la ligne de musique élimine un ennemi
    std::unique_ptr<sf::SoundBuffer> lineKillsSoundBuffer;

    /// textures chargées lors du chargement de la map (présentes sur les image layers)
    std::map<std::string, std::shared_ptr<sf::Texture>> autoloadedTextures;

    /// sprites utilisés pour la décoration du niveau (présents sur les image layers)
    std::vector<sf::Sprite> levelDecorations;

    // UI
    std::shared_ptr<sf::Texture> finishLineUITexture;
    std::shared_ptr<sf::Texture> boatProgressUITexture;
    sf::Sprite finishLineSprite;
    sf::Sprite boatProgressSprite;

    float finishLineX = 0.0f;
    b2Body* levelCollisions;

    void renderHealthBar();
    void renderProgressBar();

    void loadLayer(tmx::Layer& layer);
    void loadCollision(const tmx::Object& obj);

    /// Charge une texture avec mise en cache, utilisé pour ne pas dupliquer les textures lors du chargement de la map
    shared_ptr<sf::Texture> autoloadTexture(std::string path);

public:
    /// Ajoute une fixture correspondant à l'objet donné (ie même forme) au rigidbody 'body'
    static b2Fixture * createFixture(b2Body *body, const tmx::Object &obj);

    explicit BoatSegment(Game& game);

    /// Appelé toutes les TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
    void update() override;

    /// Appelé autant que possible pour afficher le jeu
    /// @param renderTarget fenêtre sur laquelle on dessine
    /// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
    /// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
    void render(sf::RenderWindow& renderTarget, float partialTick) override;

    /// Appelé au début d'un clic
    void mousePressed(int x, int y, sf::Mouse::Button button) override;

    /// Appelé à la fin d'un clic
    void mouseReleased(int x, int y, sf::Mouse::Button button) override;

    // Appelé lorsque la souris est déplacée avec un clic enfoncé (appelé après mouseMoved)
    void mouseDrag(int x, int y, int dx, int dy, sf::Mouse::Button button) override;

    void shutdown() override;

    ~BoatSegment();
};