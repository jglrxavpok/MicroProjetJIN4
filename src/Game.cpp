//
// Created by jglrxavpok on 28/05/2020.
//

#include "elements/LoopingBackground.h"
#include "elements/BoatElement.h"
#include "elements/ShoreElement.h"
#include "Game.h"
#include "Scene.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include "math.h"

unique_ptr<Scene> scene = nullptr;

std::shared_ptr<sf::Texture> Game::loadTexture(std::string path) {
    std::shared_ptr<sf::Texture> tex = make_shared<sf::Texture>();
    if(!tex->loadFromFile(path)) {
        std::cerr << "Failed to load " << path << std::endl;
    }
    return tex;
}

// FIXME: tmp
b2Body* rigidbody2;

void Game::init() {
    unique_ptr<SceneElement> background = make_unique<LoopingBackground>(loadTexture("resources/boat_background.png"));
    scene = make_unique<Scene>(move(background));

    scene->addElement(make_unique<ShoreElement>(0.0f));
    scene->addElement(make_unique<ShoreElement>(822.5f));
    scene->addElement(make_unique<BoatElement>());

    // FIXME: anything below this line is purely for testing purposes
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.angle = -M_PI/2.5f;
    bodyDef.position.Set(1000.0f, 450.0f);
    rigidbody2 = scene->getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape line;
    line.SetAsBox(100.0f, 10.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &line;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    rigidbody2->CreateFixture(&fixtureDef);
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

    // FIXME: tmp
    auto& defaultView = renderTarget.getDefaultView();
    renderTarget.setView(scene->getRenderView());
    if(rigidbody2) {
        auto s = sf::RectangleShape(sf::Vector2f(200.0f, 20.0f));
        s.setOrigin(100.0f, 10.0f);
        s.setRotation(rigidbody2->GetAngle()*180.0f/M_PI);
        s.setPosition(rigidbody2->GetPosition().x, rigidbody2->GetPosition().y);
        s.setFillColor(sf::Color::Black);
        renderTarget.draw(s);
    }
    renderTarget.setView(defaultView);
}