//
// Created by jglrxavpok on 29/05/2020.
//

#include "TmpLineElement.h"
#include "Scene.h"
#define _USE_MATH_DEFINES
#include "math.h"


TmpLineElement::TmpLineElement(float x, float y): startX(x), startY(y) {}

void TmpLineElement::onAddition(Scene &scene) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.angle = -M_PI/2.5f;

    bodyDef.position.Set(startX, startY);
    rigidbody = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape line;
    line.SetAsBox(100.0f, 10.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &line;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    rigidbody->CreateFixture(&fixtureDef);
}

void TmpLineElement::update(float elapsedTime) {}

void TmpLineElement::render(sf::RenderWindow &target, float partialTick) {
    auto s = sf::RectangleShape(sf::Vector2f(200.0f, 20.0f));
    s.setOrigin(100.0f, 10.0f);
    s.setRotation(rigidbody->GetAngle()*180.0f/M_PI);
    s.setPosition(rigidbody->GetPosition().x, rigidbody->GetPosition().y);
    s.setFillColor(sf::Color::Black);
    target.draw(s);
}
