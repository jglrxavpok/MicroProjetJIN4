//
// Created by jglrxavpok on 29/05/2020.
//

#include "PlayerLineElement.h"

#include <utility>
#include "Scene.h"
#define _USE_MATH_DEFINES
#include "math.h"


PlayerLineElement::PlayerLineElement(std::shared_ptr<MusicLine> parent, float startX, float startY, float endX, float endY):
    parent(std::move(parent)), startX(startX), startY(startY), endX(endX), endY(endY) {
    float dx = endX-startX;
    float dy = endY-startY;
    length = sqrt(dx*dx+dy*dy);

    life = MAX_LIFETIME;
}

void PlayerLineElement::onAddition(Scene &scene) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.angle = atan2((endY-startY), endX-startX);

    bodyDef.position.Set((startX+endX)/2.0f, (startY+endY)/2.0f);
    rigidbody = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape line;
    line.SetAsBox(length/2.0f, 10.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &line;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    rigidbody->CreateFixture(&fixtureDef);
}

void PlayerLineElement::update(float elapsedTime) {
    life -= elapsedTime;
    if(life <= 0) {
        scheduleForRemoval();
    }
}

void PlayerLineElement::render(sf::RenderWindow &target, float partialTick) {
    auto s = sf::RectangleShape(sf::Vector2f(length, 20.0f));
    s.setOrigin(length/2.0f, 10.0f);
    s.setRotation(rigidbody->GetAngle()*180.0f/M_PI);
    s.setPosition(rigidbody->GetPosition().x, rigidbody->GetPosition().y);

    uint8_t alpha = (uint8_t) floor(life/MAX_LIFETIME*255);
    s.setFillColor(sf::Color{0,0,0,alpha});
    target.draw(s);
}

PlayerLineElement::~PlayerLineElement() {
    if(rigidbody && rigidbody->GetWorld()) {
        rigidbody->GetWorld()->DestroyBody(rigidbody);
    }
}
