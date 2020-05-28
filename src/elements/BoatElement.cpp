//
// Created by jglrxavpok on 28/05/2020.
//

#include "BoatElement.h"
#include "Scene.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <cmath>
#include <box2d/b2_joint.h>

BoatElement::BoatElement(): SceneElement() {
    pointyPart.setFillColor(sf::Color::Green);
    squarePart.setFillColor(sf::Color::Green);

    pointyPart.setOrigin(pixelWidth/2.0f, pixelWidth/2.0f);
    squarePart.setOrigin(pixelLength/2.0f, pixelWidth/2.0f);
}

// FIXME: tmp
b2Body* rigidbody2;

void BoatElement::onAddition(Scene &scene) {
    b2BodyDef bodyDef; // bodyDef n'est utilisé que pendant la construction du b2Body, donc on peut le mettre sur le stack
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(200.0f, 400.0f);
    rigidbody = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(pixelLength/2.0f, pixelWidth/2.0f);


    // fixture pour l'arrière de la barque
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 100.0f;
    fixtureDef.friction = 0.0f;
    rigidbody->CreateFixture(&fixtureDef);

    // fixture pour l'avant de la barque
    b2PolygonShape triangle;
    vector<b2Vec2> points;
    points.emplace_back(b2Vec2(pixelLength/2.0f, -pixelWidth/2.0f));
    points.emplace_back(b2Vec2(pixelLength/2.0f, pixelWidth/2.0f));
    points.emplace_back(b2Vec2(pixelLength/2.0f + sqrt(3.0f)*pixelWidth/2.0f, 0.0f));
    triangle.Set(points.data(), points.size());

    fixtureDef.shape = &triangle;
    fixtureDef.density = 100.0f;
    fixtureDef.friction = 0.0f;
    rigidbody->CreateFixture(&fixtureDef);

    // FIXME: anything below this line is purely for testing purposes
    bodyDef.type = b2_kinematicBody;
    bodyDef.angularVelocity = 0.0f;
    bodyDef.angle = -M_PI/2.5f;
    bodyDef.position.Set(1000.0f, 400.0f);
    bodyDef.linearVelocity.x = -200.0f;
    rigidbody2 = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape line;
    line.SetAsBox(100.0f, 10.0f);

    fixtureDef.shape = &line;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    rigidbody2->CreateFixture(&fixtureDef);
}

void BoatElement::update(float elapsedTime) {
    static float time = 0.0f;
    time += elapsedTime;

    auto transform = rigidbody->GetTransform();
    transform.p.x = 200.0f;
    rigidbody->SetTransform(transform.p, transform.q.GetAngle());
    position.x = transform.p.x;
    position.y = transform.p.y;
    angle = transform.q.GetAngle();
}

void BoatElement::render(sf::RenderWindow &target, float partialTick) {
    if(rigidbody) {
        squarePart.setPosition(position.x, position.y);
        squarePart.setRotation(angle/M_PI*180.0f);
        pointyPart.setRotation(angle/M_PI*180.0f + 90.0f);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);

        float dx = cosAngle * (pixelLength/2.0f+pixelWidth/4.0f);
        float dy = sinAngle * (pixelLength/2.0f+pixelWidth/4.0f);
        pointyPart.setPosition(dx+position.x, dy+position.y);
    }

    target.draw(squarePart);
    target.draw(pointyPart);

    // FIXME: tmp
    if(rigidbody2) {
        auto s = sf::RectangleShape(sf::Vector2f(200.0f, 20.0f));
        s.setOrigin(100.0f, 10.0f);
        s.setRotation(rigidbody2->GetAngle()*180.0f/M_PI);
        s.setPosition(rigidbody2->GetPosition().x, rigidbody2->GetPosition().y);
        s.setFillColor(sf::Color::Black);
        target.draw(s);
    }
}
