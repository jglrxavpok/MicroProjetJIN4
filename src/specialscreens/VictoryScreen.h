//
// Created by jglrxavpok on 05/06/2020.
//
#pragma once
#include "GameplaySegment.h"
#include "Game.h"
#include "boat_segment/elements/FixedBackground.h"

class VictoryScreen: public GameplaySegment {
private:
    std::shared_ptr<sf::Texture> singleNotesTexture;
    std::shared_ptr<sf::Texture> doubleNotesTexture;

    void placeNote(std::shared_ptr<MusicLine> line, float x, float y, bool single);
public:
    explicit VictoryScreen(Game& game);

    void render(sf::RenderWindow &renderTarget, float partialTick) override;

    void keyReleased(sf::Event::KeyEvent event) override;

    void update() override;

    void shutdown() override {}
};
