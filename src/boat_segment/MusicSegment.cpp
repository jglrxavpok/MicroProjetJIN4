//
// Created by gwenser
//

#include "MusicSegment.h"
#include "specialscreens/GameOverScreen.hpp"
#include <specialscreens\TransitionScreen.hpp>
#include <boat_segment\BoatSegment.h>

MusicSegment::MusicSegment(Game& game): GameplaySegment(game) {
    for (auto& note : names) {
        notes[note] = std::make_shared<Sound>(note);
    }

    keyboardImage = Game::loadTexture("resources/azerty_keyboard.png");

    keyBoardSprite.setTexture(*keyboardImage);

    keySprite.setTexture(*noKeyImage);
    for (int k = 0; k < NBLIFE; k++) {
        livesSprites.push_back(std::make_unique<sf::Sprite>());
        livesSprites[k]->setTexture(*heart);
        livesSprites[k]->setScale(sf::Vector2f(0.3f, 0.3f));
        livesSprites[k]->setPosition(sf::Vector2f(500+k*200, 600));
    }

    font.loadFromFile("resources/fonts/segoeprb.ttf");
    pourcentagePrint.setString(((char*)pourcentage));
    pourcentagePrint.setPosition(sf::Vector2f(900, 100));
    pourcentagePrint.setColor(sf::Color::Cyan);
    pourcentagePrint.setCharacterSize(30);
    pourcentagePrint.setFont(font);
}

void MusicSegment::playSound(std::string note) {
    notes[note]->play();
}

void MusicSegment::checkNote(std::string note) {
    if (!gameOver) {
        playerPlayed = 1;
        lastPressKey = notes[note];
        if (lastPressKey->getState() == keyState::blinking) {
            lastPressKey->setState(keyState::right);
        }
        else {
            lastPressKey->setState(keyState::wrong);
            hurtPlayer();
        }
    }
}

void MusicSegment::update() {
    if(gameOver) {
        game.setGameplay(make_unique<GameOverScreen<MusicSegment>>(game));
        return;
    }

    ticks++;
    this->playMusic();
    if (ticks % TEMPO > TEMPO / 10) {
        for (int life = 0; life < lives; life++) {
            livesSprites[life]->setTexture(*heart);
        }
    }
    else {
        for (int life = 0; life < lives; life++) {
            livesSprites[life]->setTexture(*beatingHeart);
        }
    }
}

void MusicSegment::playMusic() {
    if (notePlay != ticks / TEMPO && notePlay < music.size()) {
        //std::cout << lives << std::endl;
        //notes[music[notePlay]]->play();

        if (!playerPlayed) {
            lives--;
            livesSprites[lives]->setTexture(*deadHeart);
        }
        if (lives <= 0) {
            gameOver = 1;
        }

        playerPlayed = 0;
        if (!gameOver) {
            if (lastPressKey) {
                lastPressKey->setState(keyState::nothing);
            }
            lastPressKey = notes[music[notePlay]];
            lastPressKey->setState(keyState::blinking);
            pourcentage = notePlay / music.size();
            notePlay++;
        }
    }
    if (!gameOver && notePlay == music.size() && win == 0) {
        win = 1;
        vector<wstring> text = { L"Orphée a réussi à charmer le Passeur et Cerbère.", L"Il arpente ensuite le Styx pour rejoindre sa bien-aimée." };
        game.setGameplay(move(make_unique<TransitionScreen<BoatSegment>>(game, text)));
    }
}

void MusicSegment::render(sf::RenderWindow& renderTarget, float partialTick) {
    renderTarget.clear(sf::Color::White);
    renderTarget.draw(keyBoardSprite);
    if (lastPressKey) {
        lastPressKey->render(renderTarget, partialTick, keySprite, noKeyImage);
    }
    renderTarget.draw(keySprite);
    for (int lifeSprite = 0; lifeSprite < NBLIFE; lifeSprite++) {
        renderTarget.draw(*livesSprites[lifeSprite]);
    }
    pourcentagePrint.setString(((char*)pourcentage));
    renderTarget.draw(pourcentagePrint);
}

void MusicSegment::keyPressed(sf::Event::KeyEvent event) {
    if (event.code == sf::Keyboard::A) {
        playSound("A");
        checkNote("A");
    }
    else if (event.code == sf::Keyboard::B) {
        playSound("B");
        checkNote("B");
    }
    else if (event.code == sf::Keyboard::C) {
        playSound("C");
        checkNote("C");
    }
    else if (event.code == sf::Keyboard::D) {
        playSound("D");
        checkNote("D");
    }
    else if (event.code == sf::Keyboard::E) {
        playSound("E");
        checkNote("E");
    }
    else if (event.code == sf::Keyboard::F) {
        playSound("F");
        checkNote("F");
    }
    else if (event.code == sf::Keyboard::G) {
        playSound("G");
        checkNote("G");
    }
    else {
        hurtPlayer();
    }
}

void MusicSegment::hurtPlayer() {
    lives--;
    if(lives >= 0) {
        livesSprites[lives]->setTexture(*deadHeart);
    }
    if (lives <= 0) {
        gameOver = 1;
    }
}

void MusicSegment::shutdown() {

}
