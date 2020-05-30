//
// Created by jglrxavpok on 28/05/2020.
//

#include "elements/LoopingBackground.h"
#include "elements/BoatElement.h"
#include "elements/ShoreElement.h"
#include "elements/PlayerLineElement.h"
#include "Game.h"
#include "Scene.h"
#include <iostream>
#include <elements/EnemyElement.h>

#define _USE_MATH_DEFINES
#include "math.h"

std::shared_ptr<sf::Texture> Game::loadTexture(std::string path) {
    std::shared_ptr<sf::Texture> tex = make_shared<sf::Texture>();
    if(!tex->loadFromFile(path)) {
        std::cerr << "Failed to load " << path << std::endl;
    }
    return tex;
}

Game::Game(sf::RenderWindow& window): renderTarget(window) {
    badGuyTexture = loadTexture("resources/textures/bad_guy.png");
    unique_ptr<SceneElement> background = make_unique<LoopingBackground>(loadTexture("resources/textures/boat_background.png"));
    scene = make_unique<Scene>(move(background));

    scene->addElement(make_unique<ShoreElement>(0.0f));
    scene->addElement(make_unique<ShoreElement>(822.5f));
    scene->addElement(make_unique<BoatElement>());
}

void Game::spawnEnemy() {
    auto enemy = make_unique<EnemyElement>(badGuyTexture);
    float centerY = rng.rand(ShoreElement::HEIGHT, 822.5f);
    auto coords = renderTarget.mapPixelToCoords(sf::Vector2i(1600-200, (int)centerY), scene->getRenderView());

    enemy->getPosition() = coords;
    scene->addElement(move(enemy));
}

void Game::update() {
    if(scene) {
        scene->updateAll(Game::TARGET_UPDATE_PERIOD);

        time += Game::TARGET_UPDATE_PERIOD;

        if(time >= ENEMY_SPAWN_PERIOD) {
            spawnEnemy();
            time -= ENEMY_SPAWN_PERIOD;
        }
    }
}

void Game::render(sf::RenderWindow& renderTarget, float partialTick) {
    renderTarget.clear(sf::Color::White);

    if(scene) {
        scene->renderAll(renderTarget, partialTick);
    }
}

void Game::updateMousePos(int x, int y) {
    mousePosX = x;
    mousePosY = y;
}

int lastX = 0;
int lastY = 0;

void Game::mousePressed(int x, int y, sf::Mouse::Button button) {
    updateMousePos(x, y);
    buttonPressed[button] = true;
    lastX = x;
    lastY = y;
    currentMusicLine = make_shared<MusicLine>(scene);

    // TODO: propager l'event

}

void Game::mouseReleased(int x, int y, sf::Mouse::Button button) {
    updateMousePos(x, y);
    buttonPressed[button] = false;
    currentMusicLine = nullptr;

    // TODO: propager l'event
}

void Game::mouseMoved(int x, int y) {
    int prevX = mousePosX;
    int prevY = mousePosY;
    int dx = prevX-x;
    int dy = prevY-y;
    updateMousePos(x, y);
    // TODO: propager l'event

    for (int buttonIndex = 0; buttonIndex < sf::Mouse::Button::ButtonCount; buttonIndex++) {
        auto button = (sf::Mouse::Button) buttonIndex;
        if(buttonPressed[button]) {
            // TODO: propager l'event de drag

            // FIXME: tmp
            if(button == sf::Mouse::Left) {
                int deltaX = x - lastX;
                int deltaY = y - lastY;
                if(deltaX*deltaX+deltaY*deltaY >= 20*20) { // plus de 50 pixels de distance
                    auto startCoords = renderTarget.mapPixelToCoords(sf::Vector2i(lastX, lastY), scene->getRenderView());
                    auto endCoords = renderTarget.mapPixelToCoords(sf::Vector2i(x, y), scene->getRenderView());
                    if(currentMusicLine) {
                        currentMusicLine->addLine(startCoords.x, startCoords.y, endCoords.x, endCoords.y);
                    }

                    lastX = x;
                    lastY = y;
                }
            }
        }
    }
}

void Game::shutdown() {

}