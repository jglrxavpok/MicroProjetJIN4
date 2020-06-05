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
    notes[note]->play();
}

void MusicSegment::checkNote(std::string note) {
    if (!gameOver) {
        lastPressKey = notes[note];
        if (lastPressKey->getState() == keyState::blinking) {
            lastPressKey->setState(keyState::right);
        }
        else {
            lastPressKey->setState(keyState::wrong);
            lives--;
            if (lives <= 0) {
                gameOver = 1;
            }
        }
    }
}

void MusicSegment::update() {
    ticks++;
    if (!gameOver) {
        this->playMusic();
    }
}

void MusicSegment::playMusic() {
    if (notePlay != ticks / TEMPO && notePlay < music.size()) {
        //notes[music[notePlay]]->play();
        if (lastPressKey) {
            lastPressKey->setState(keyState::nothing);
        }
        lastPressKey = notes[music[notePlay]];
        lastPressKey->setState(keyState::blinking);
        notePlay++;
    }
}

void MusicSegment::render(sf::RenderWindow& renderTarget, float partialTick) {
    renderTarget.clear(sf::Color::White);
    renderTarget.draw(keyBoardSprite);
    if (lastPressKey) {
        lastPressKey->render(renderTarget, partialTick, keySprite, noKeyImage);
    }
    renderTarget.draw(keySprite);
}

void MusicSegment::keyPressed(sf::Event::KeyEvent event) {
    if (event.code == sf::Keyboard::A) {
        playSound("A");
        checkNote("A");
    }
    if (event.code == sf::Keyboard::B) {
        playSound("B");
        checkNote("B");
    }
    if (event.code == sf::Keyboard::C) {
        playSound("C");
        checkNote("C");
    }
    if (event.code == sf::Keyboard::D) {
        playSound("D");
        checkNote("D");
    }
    if (event.code == sf::Keyboard::E) {
        playSound("E");
        checkNote("E");
    }
    if (event.code == sf::Keyboard::F) {
        playSound("F");
        checkNote("F");
    }
    if (event.code == sf::Keyboard::G) {
        playSound("G");
        checkNote("G");
    }
}

void MusicSegment::shutdown() {

}
