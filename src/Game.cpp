//
// Created by jglrxavpok on 28/05/2020.
//

#include "Game.h"


std::unique_ptr<sf::SoundBuffer> Game::loadBuffer(std::string path) {
    std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique< sf::SoundBuffer>();
    if (!buffer->loadFromFile(path)) {
        std::cout << "Impossible d'ouvrir le fichier : " << path << std::endl;
    }
    return buffer;
}

std::shared_ptr<sf::Sound> Game::loadSound(std::unique_ptr<sf::SoundBuffer>& buffer) {
    std::shared_ptr<sf::Sound> sound = std::make_shared<sf::Sound>();
    sound->setBuffer(*buffer);
    sound->setVolume(100.f);
    return sound;
}

std::shared_ptr<sf::Image> Game::loadImage(std::string path) {
    std::shared_ptr<sf::Image> image = std::make_shared<sf::Image>();
    if (!image->loadFromFile(path)) {
        std::cerr << "Impossible d'ouvrir : " << path << std::endl;
    }
    return image;
}

Game::Game() {
    buffers[Sound::A] = std::move(loadBuffer("resources/A.wav"));
    buffers[Sound::B] = std::move(loadBuffer("resources/B.wav"));
    buffers[Sound::C] = std::move(loadBuffer("resources/C.wav"));
    buffers[Sound::D] = std::move(loadBuffer("resources/D.wav"));
    buffers[Sound::E] = std::move(loadBuffer("resources/E.wav"));
    buffers[Sound::F] = std::move(loadBuffer("resources/F.wav"));
    buffers[Sound::G] = std::move(loadBuffer("resources/G.wav"));

    sounds[Sound::A] = loadSound(buffers[Sound::A]);
    sounds[Sound::B] = loadSound(buffers[Sound::B]);
    sounds[Sound::C] = loadSound(buffers[Sound::C]);
    sounds[Sound::D] = loadSound(buffers[Sound::D]);
    sounds[Sound::E] = loadSound(buffers[Sound::E]);
    sounds[Sound::F] = loadSound(buffers[Sound::F]);
    sounds[Sound::G] = loadSound(buffers[Sound::G]);
}

void Game::update() {
    // TODO
}

void Game::playSound(Sound sound) {
    switch (sound) {
        case Sound::A:
            sounds[Sound::A]->play();
            break;

        case Sound::B:
            sounds[Sound::B]->play();
            break;

        case Sound::C:
            sounds[Sound::C]->play();
            break;

        case Sound::D:
            sounds[Sound::D]->play();
            break;

        case Sound::E:
            sounds[Sound::E]->play();
            break;

        case Sound::F:
            sounds[Sound::F]->play();
            break;

        case Sound::G:
            sounds[Sound::G]->play();
            break;
    }
}

void Game::render(sf::RenderWindow& renderTarget, float partialTick) {
    renderTarget.clear(sf::Color::White);

    // TODO
}