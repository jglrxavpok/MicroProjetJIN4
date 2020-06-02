//
// Created by gwenser
//

#include "MusicSegment.h"

MusicSegment::MusicSegment(Game& game): GameplaySegment(game) {
    for (auto& note : names) {
        notes[note] = std::make_shared<Sound>(note);
    }

    keyboardImage = Game::loadTexture("resources/azerty_keyboard.png");

    keyBoardSprite.setTexture(*keyboardImage);

    keySprite.setTexture(*noKeyImage);
}

void MusicSegment::playSound(std::string note) {
    lastPressKey = notes[note];
    lastPressKey->play();
}

void MusicSegment::update() {
    // TODO
}

void MusicSegment::playMusic() {
    //music
}

void MusicSegment::render(sf::RenderWindow& renderTarget, float partialTick) {
    renderTarget.clear(sf::Color::White);
    renderTarget.draw(keyBoardSprite);
    renderTarget.draw(keySprite);
}

void MusicSegment::keyPressed(sf::Event::KeyEvent event) {
    if (event.code == sf::Keyboard::A) {
        playSound("A");
    }
    if (event.code == sf::Keyboard::B) {
        playSound("B");
    }
    if (event.code == sf::Keyboard::C) {
        playSound("C");
    }
    if (event.code == sf::Keyboard::D) {
        playSound("D");
    }
    if (event.code == sf::Keyboard::E) {
        playSound("E");
    }
    if (event.code == sf::Keyboard::F) {
        playSound("F");
    }
    if (event.code == sf::Keyboard::G) {
        playSound("G");
    }
}

void MusicSegment::shutdown() {

}
