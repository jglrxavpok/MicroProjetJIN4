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
        livesSprites[k]->setPosition(sf::Vector2f(500+k*200, 300));
    }

    font.loadFromFile("resources/fonts/segoeprb.ttf");
    pourcentagePrint.setString("0%");
    pourcentagePrint.setPosition(sf::Vector2f(900, 100));
    pourcentagePrint.setFillColor(sf::Color::Cyan);
    pourcentagePrint.setCharacterSize(60);
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
        if (!playerPlayed) {
            hurtPlayer();
        }

        playerPlayed = 0;
        if (!gameOver) {
            if (lastPressKey) {
                lastPressKey->setState(keyState::nothing);
            }
            lastPressKey = notes[music[notePlay]];
            lastPressKey->setState(keyState::blinking);
            pourcentage = (notePlay*100) / music.size();
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

    renderIncomingNotes();
    for (int lifeSprite = 0; lifeSprite < NBLIFE; lifeSprite++) {
        renderTarget.draw(*livesSprites[lifeSprite]);
    }
    pourcentagePrint.setString(std::to_string(pourcentage)+"%");
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

void MusicSegment::renderIncomingNotes() {
    const float noteSize = 64.0f;
    const float xSpacing = 5.0f;
    int fitInWidth = renderTarget.getSize().x / (noteSize+xSpacing)+1; // +1 pour dessiner la note qui est à moitié hors de l'écran à droite

    size_t noteCount = min(music.size()-(notePlay-1), (size_t) fitInWidth);
    const float ySpacing = -32.0f;
    const float height = 7.0f * (noteSize + ySpacing);

    float startY = 600.0f;
    float offsetX = musicKeyTexture->getSize().x;

    // le code est pas hyper lisible et probablement horrible à lire pour un musicien
    // j'ai pas fait de solfège et je fais comme je peux
    //
    // Pourquoi les notes sont pas sur les lignes?

    // dessin des "lignes de partition"
    for (int j = -4; j < 10-4; j += 2) {
        float relativeY = j/7.0f*height + noteSize/2.0f;
        float y = startY+relativeY;
        sf::RectangleShape rect{sf::Vector2f(renderTarget.getSize().x, 3.0f)};
        rect.setPosition(0.0f, y);
        rect.setFillColor(sf::Color::Black);
        renderTarget.draw(rect);
    }

    // dessin des notes
    for (int i = 0; i < noteCount; ++i) {
        if(notePlay == 0)
            continue;
        std::string& noteStr = music[notePlay+i -1];
        auto& note = notes[noteStr];
        auto noteIndex = distance(names.begin(), find(names.begin(), names.end(), noteStr));
        float progressToNextNote = (ticks % TEMPO) / (float)TEMPO;
        float x = offsetX + (i-progressToNextNote)*(noteSize + xSpacing);
        float relativeY = noteIndex/7.0f*height;
        float y = startY+relativeY;
        note->renderSingle(renderTarget, x, y);
    }

    // rendu de la clé
    sf::Sprite keySprite{*musicKeyTexture};
    keySprite.setPosition(0.0f, startY+height/2.0f-6.0/14.0f*height-musicKeyTexture->getSize().y/2.0f); // centré verticalement sur les lignes
    renderTarget.draw(keySprite);
}
