//
// Created by jglrxavpok on 28/05/2020.
//

#include "Game.h"


std::unique_ptr<sf::SoundBuffer> loadBuffer(std::string path) {
    std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique< sf::SoundBuffer>();
    if (!buffer->loadFromFile(path)) {
        std::cout << "Impossible d'ouvrir le fichier : " << path << std::endl;
    }
    return buffer;
}

std::shared_ptr<sf::Sound> loadSound(std::unique_ptr<sf::SoundBuffer>& buffer) {
    std::shared_ptr<sf::Sound> sound = std::make_shared<sf::Sound>();
    sound->setBuffer(*buffer);
    sound->setVolume(100.f);
    return sound;
}

std::shared_ptr<sf::Texture> loadImage(std::string path) {
    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        std::cerr << "Impossible d'ouvrir : " << path << std::endl;
    }
    return texture;
}

Game::Game() {
    for (auto& note : names) {
        notes[note] = std::make_shared<Sound>(note);
    }

    keyboardImage = loadImage("resources/azerty_keyboard.png");

    keyBoardSprite.setTexture(*keyboardImage);

    keySprite.setTexture(*noKeyImage);
}

void Game::playSound(std::string note) {
    lastPressKey = notes[note];
    lastPressKey->play();
}

void Game::update() {
    // TODO
}

void Game::playMusic() {
    //music
}

void Game::render(sf::RenderWindow& renderTarget, float partialTick) {
    renderTarget.clear(sf::Color::White);
    renderTarget.draw(keyBoardSprite);
    renderTarget.draw(keySprite);
}