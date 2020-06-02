//
// Created by jglrxavpok on 02/06/2020.
//

#include "Game.h"

Game::Game(sf::RenderWindow &window): renderTarget(window) {}

void Game::update() {
    if(currentSegment) {
        currentSegment->update();
    }
}

void Game::render(sf::RenderWindow &renderTarget, float partialTick) {
    if(currentSegment) {
        currentSegment->render(renderTarget, partialTick);
    }
}

void Game::shutdown() {
    if(currentSegment) {
        currentSegment->shutdown();
    }
}

sf::RenderWindow& Game::getRenderTarget() {
    return renderTarget;
}

std::shared_ptr<sf::Texture> Game::loadTexture(std::string path) {
    std::shared_ptr<sf::Texture> tex = make_shared<sf::Texture>();
    if(!tex->loadFromFile(path)) {
        std::cerr << "Failed to load " << path << std::endl;
    }
    return tex;
}

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

void Game::updateMousePos(int x, int y) {
    mousePosX = x;
    mousePosY = y;
}

void Game::mousePressed(int x, int y, sf::Mouse::Button button) {
    updateMousePos(x, y);
    buttonPressed[button] = true;
    if(currentSegment) {
        currentSegment->mousePressed(x, y, button);
    }
}

void Game::keyPressed(sf::Event::KeyEvent event) {
    if(currentSegment) {
        currentSegment->keyPressed(event);
    }
}

void Game::keyReleased(sf::Event::KeyEvent event) {
    if(currentSegment) {
        currentSegment->keyReleased(event);
    }
}

void Game::mouseReleased(int x, int y, sf::Mouse::Button button) {
    updateMousePos(x, y);
    buttonPressed[button] = false;

    if(currentSegment) {
        currentSegment->mouseReleased(x,y,button);
    }
}

void Game::mouseMoved(int x, int y) {
    int prevX = mousePosX;
    int prevY = mousePosY;
    int dx = prevX-x;
    int dy = prevY-y;
    updateMousePos(x, y);
    if(currentSegment) {
        currentSegment->mouseMoved(x,y);
    }

    if(currentSegment) {
        for (int buttonIndex = 0; buttonIndex < sf::Mouse::Button::ButtonCount; buttonIndex++) {
            auto button = (sf::Mouse::Button) buttonIndex;
            if (buttonPressed[button]) {
                if (button == sf::Mouse::Left) {
                    currentSegment->mouseDrag(x, y, dx, dy, button);
                }
            }
        }
    }
}

void Game::setGameplay(unique_ptr<GameplaySegment> gameplay) {
    currentSegment = move(gameplay);
}
