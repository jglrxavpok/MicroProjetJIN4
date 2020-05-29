//
// Created by jglrxavpok on 28/05/2020.
//

#include "elements/LoopingBackground.h"
#include "elements/BoatElement.h"
#include "elements/ShoreElement.h"
#include "elements/TmpLineElement.h"
#include "Game.h"
#include "Scene.h"
#include <iostream>

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
    unique_ptr<SceneElement> background = make_unique<LoopingBackground>(loadTexture("resources/boat_background.png"));
    scene = make_unique<Scene>(move(background));

    scene->addElement(make_unique<ShoreElement>(0.0f));
    scene->addElement(make_unique<ShoreElement>(822.5f));
    scene->addElement(make_unique<BoatElement>());
}

void Game::update() {
    if(scene) {
        scene->updateAll(Game::TARGET_UPDATE_PERIOD);
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

void Game::mousePressed(int x, int y, sf::Mouse::Button button) {
    updateMousePos(x, y);
    buttonPressed[button] = true;

    // TODO: propager l'event

    // TODO: tmp
    if(button == sf::Mouse::Left) {
        auto coords = renderTarget.mapPixelToCoords(sf::Vector2i(x, y), scene->getRenderView());
        scene->addElement(make_unique<TmpLineElement>(coords.x, coords.y));
    }
}

void Game::mouseReleased(int x, int y, sf::Mouse::Button button) {
    updateMousePos(x, y);
    buttonPressed[button] = false;

    // TODO: propager l'event
}

void Game::mouseMoved(int x, int y) {
    int dx = mousePosX-x;
    int dy = mousePosY-y;
    updateMousePos(x, y);
    // TODO: propager l'event

    for (int buttonIndex = 0; buttonIndex < sf::Mouse::Button::ButtonCount; buttonIndex++) {
        auto button = (sf::Mouse::Button) buttonIndex;
        if(buttonPressed[button]) {
            // TODO: propager l'event de drag
        }
    }
}

void Game::shutdown() {

}