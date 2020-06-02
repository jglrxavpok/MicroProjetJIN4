//
// Created by jglrxavpok on 28/05/2020.
//

#include "BoatElement.h"
#include "Scene.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <cmath>
#include <utility>
#include <box2d/b2_joint.h>

BoatElement::BoatElement(GameState& gameState): gameState(gameState), SceneElement() {
    pointyPart.setFillColor(sf::Color::Green);
    squarePart.setFillColor(sf::Color::Green);

    pointyPart.setOrigin(pixelWidth/2.0f, pixelWidth/2.0f);
    squarePart.setOrigin(pixelLength/2.0f, pixelWidth/2.0f);
}

void BoatElement::onAddition(Scene &scene) {
    this->scene = &scene; // permet de garder une référence à la scène pour changer la vue de rendu

    b2BodyDef bodyDef; // bodyDef n'est utilisé que pendant la construction du b2Body, donc on peut le mettre sur le stack
    bodyDef.type = b2_dynamicBody;
    bodyDef.angularDamping = angularDamping;
    bodyDef.position.Set(200.0f, 450.0f);
    bodyDef.userData = this;
    rigidbody = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(pixelLength/2.0f, pixelWidth/2.0f);


    // fixture pour l'arrière de la barque
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1000.0f;
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
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.0f;
    rigidbody->CreateFixture(&fixtureDef);
}

void BoatElement::update(float elapsedTime) {
    if(scene) {
        sf::View& renderView = scene->getRenderView();
        if(auto& background = scene->getBackground()) {
            background->getPosition().x = position.x;
        }
        renderView.setCenter(position.x+renderView.getSize().x/2.0f-200.0f, renderView.getSize().y/2.0f);
    }

    static float time = 0.0f;
    time += elapsedTime;

    if(rigidbody) {
        b2Vec2 impulse;
        float f = linearImpulseFactor * rigidbody->GetMass();
        impulse.x = cos(angle) * f;
        impulse.y = sin(angle) * f;
        rigidbody->ApplyLinearImpulseToCenter(impulse, true);

        rigidbody->ApplyAngularImpulse(-angularImpulseFactor*rigidbody->GetInertia() * rigidbody->GetAngle(), true);

        auto transform = rigidbody->GetTransform();
        position.x = transform.p.x;
        position.y = transform.p.y;
        angle = transform.q.GetAngle();
    }
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
}

void BoatElement::damage(float amount) {
    gameState.damage(amount);
}
