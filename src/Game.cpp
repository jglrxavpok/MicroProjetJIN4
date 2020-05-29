//
// Created by jglrxavpok on 28/05/2020.
//

#include "Game.h"

std::shared_ptr<sf::Sound> Game::loadSound(std::string path) {
    std::shared_ptr<sf::Sound> sound = std::make_shared<sf::Sound>();
    std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique< sf::SoundBuffer>();
    if (!buffer->loadFromFile(path)) {
        std::cout << "Impossible d'ouvrir le fichier : " << path << std::endl;
    }
    sound->setBuffer(*buffer);
    sound->setVolume(100.f);
    return sound;
}

Game::Game() {
    soundA = loadSound("resources/A.wav");
    soundB = loadSound("resources/B.wav");
    soundC = loadSound("resources/C.wav");
    soundD = loadSound("resources/D.wav");
    soundE = loadSound("resources/E.wav");
    soundF = loadSound("resources/F.wav");
    soundG = loadSound("resources/G.wav");
}

void Game::update() {
    // TODO
}

void Game::playSound(Sound sound) {
    switch (sound) {
        case Sound::A:
            soundA->play();
            break;
    }
}

void Game::render(sf::RenderWindow& renderTarget, float partialTick) {
    renderTarget.clear(sf::Color::White);

    // TODO
}