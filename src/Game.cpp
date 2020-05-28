//
// Created by jglrxavpok on 28/05/2020.
//

#include "elements/LoopingBackground.h"
#include "elements/BoatElement.h"
#include "Game.h"
#include "Scene.h"
#include <iostream>

unique_ptr<Scene> scene = nullptr;

std::shared_ptr<sf::Texture> Game::loadTexture(std::string path) {
    std::shared_ptr<sf::Texture> tex = make_shared<sf::Texture>();
    if(!tex->loadFromFile(path)) {
        std::cerr << "Failed to load " << path << std::endl;
    }
    return tex;
}

void Game::init() {
    unique_ptr<SceneElement> background = make_unique<LoopingBackground>(loadTexture("resources/boat_background.png"));
    scene = make_unique<Scene>(move(background));

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