//
// Created by jglrxavpok on 05/06/2020.
//
#pragma once
#include "GameplaySegment.h"
#include "Game.h"
#include "elements/FixedBackground.h"

template<class SegmentToRetry>
class GameOverScreen: public GameplaySegment {
private:


public:
    explicit GameOverScreen(Game& game);

    void render(sf::RenderWindow &renderTarget, float partialTick) override;

    void keyReleased(sf::Event::KeyEvent event) override;

    void update() override {}

    void shutdown() override {}
};

template<class SegmentToRetry>
GameOverScreen<SegmentToRetry>::GameOverScreen(Game &game): GameplaySegment(game) {
    auto background = make_unique<FixedBackground>(Game::loadTexture("resources/textures/game_over.png"));
    scene = make_unique<Scene>(move(background));
}

template<class SegmentToRetry>
void GameOverScreen<SegmentToRetry>::render(sf::RenderWindow &renderTarget, float partialTick) {
    if(scene) {
        scene->renderAll(renderTarget, partialTick);
    }
}

template<class SegmentToRetry>
void GameOverScreen<SegmentToRetry>::keyReleased(sf::Event::KeyEvent event) {
    if(event.code == sf::Keyboard::Space) {
        game.setGameplay(move(make_unique<SegmentToRetry>(game)));
    }
}
