//
// Created by jglrxavpok on 29/05/2020.
//

#include "ShoreElement.h"
#include "Scene.h"

ShoreElement::ShoreElement(float topY): topY(topY) {}

void ShoreElement::onAddition(Scene &scene) {
    b2BodyDef shoreBodyDef;
    shoreBodyDef.type = b2_staticBody;
    shoreBodyDef.position.x = 0.0f; // la position est le centre de l'objet
    shoreBodyDef.position.y = topY+HEIGHT/2.0f; // la position est le centre de l'objet

    b2PolygonShape shoreShape;
    shoreShape.SetAsBox(100000, HEIGHT/2.0f);

    b2FixtureDef shoreFixture;
    shoreFixture.shape = &shoreShape;
    shoreFixture.friction = 0.3f;
    shoreFixture.density = 1.0f;

    scene.getPhysicsWorld().CreateBody(&shoreBodyDef)->CreateFixture(&shoreFixture);
}

void ShoreElement::update(float elapsedTime) {}

void ShoreElement::render(sf::RenderWindow &target, float partialTick) {}
