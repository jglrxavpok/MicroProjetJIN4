//
// Created by jglrxavpok on 07/06/2020.
//
#include "VictoryScreen.h"
#include "boat_segment/elements/MusicLineElement.h"

VictoryScreen::VictoryScreen(Game &game): GameplaySegment(game) {
    auto background = make_unique<FixedBackground>(Game::loadTexture("resources/textures/victory.png"));
    scene = make_unique<Scene>(move(background));

    singleNotesTexture = Game::loadTexture("resources/textures/single_notes_spritesheet.png");
    doubleNotesTexture = Game::loadTexture("resources/textures/double_notes_spritesheet.png");
    auto line = make_shared<MusicLine>(scene, singleNotesTexture, doubleNotesTexture);

    placeNote(line, 500, 170, true);
    placeNote(line, 800, 170, false);
    placeNote(line, 1100, 170, true);
}

void VictoryScreen::placeNote(std::shared_ptr<MusicLine> line, float x, float y, bool single) {
    scene->addElement(make_unique<MusicLineElement>(line, nullptr, x, y, x, y, single ? singleNotesTexture : doubleNotesTexture));
}

void VictoryScreen::render(sf::RenderWindow &renderTarget, float partialTick) {
    if(scene) {
        scene->renderAll(renderTarget, partialTick);
    }
}

void VictoryScreen::update() {
    if(scene) {
        scene->updateAll(Game::TARGET_UPDATE_PERIOD);
    }
}

void VictoryScreen::keyReleased(sf::Event::KeyEvent event) {}
