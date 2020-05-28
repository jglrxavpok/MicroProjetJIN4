//
// Created by jglrxavpok on 28/05/2020.
//

#include "BoatElement.h"
#include "Scene.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <cmath>

BoatElement::BoatElement(): SceneElement() {
    pointyPart.setFillColor(sf::Color::Green);
    squarePart.setFillColor(sf::Color::Green);

    pointyPart.setOrigin(pixelWidth/2.0f, pixelWidth/2.0f);
    squarePart.setOrigin(pixelLength/2.0f, pixelWidth/2.0f);
}

void BoatElement::onAddition(Scene &scene) {
    b2BodyDef bodyDef; // bodyDef n'est utilisé que pendant la construction du b2Body, donc on peut le mettre sur le stack
    bodyDef.type = b2_dynamicBody;
    bodyDef.angularVelocity = 10.0f;
    bodyDef.position.Set(200.0f, 400.0f);
    rigidbody = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(pixelLength/2.0f, pixelWidth/2.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    rigidbody->CreateFixture(&fixtureDef);

    rigidbody->ApplyAngularImpulse(10.0f, true);
}

void BoatElement::update(float elapsedTime) {
    static float time = 0.0f;
    time += elapsedTime;
    // TODO
    position.x = 200.0f;
    position.y = sin(time) * 200.0f + 400.0f;
}

void BoatElement::render(sf::RenderWindow &target, float partialTick) {
    if(rigidbody) {
        auto transform = rigidbody->GetTransform();
        squarePart.setPosition(transform.p.x, transform.p.y);

        float angleRad = transform.q.GetAngle();
        squarePart.setRotation(angleRad/M_PI*180.0f);
        pointyPart.setRotation(angleRad/M_PI*180.0f + 90.0f);
        float cosAngle = cos(angleRad);
        float sinAngle = sin(angleRad);

        float dx = cosAngle * (pixelLength/2.0f+pixelWidth/4.0f);
        float dy = sinAngle * (pixelLength/2.0f+pixelWidth/4.0f);
        pointyPart.setPosition(dx+transform.p.x, dy+transform.p.y);
    }

    target.draw(squarePart);
    target.draw(pointyPart);
}
