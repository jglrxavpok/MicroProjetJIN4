//
// Created by jglrxavpok on 29/05/2020.
//

#include "PlayerLineElement.h"

#include <utility>
#include "Scene.h"
#define _USE_MATH_DEFINES
#include "math.h"


PlayerLineElement::PlayerLineElement(std::shared_ptr<MusicLine> parent, std::shared_ptr<MusicLinePart> part, float startX, float startY, float endX, float endY):
    parent(std::move(parent)), part(std::move(part)), startX(startX), startY(startY), endX(endX), endY(endY) {
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
    // TODO: Briser les lignes qui rentrent en contact avec un ennemi? (à la Pokémon Ranger)

    life -= elapsedTime;
    if(life <= 0) {
        scheduleForRemoval();
    }
}

void PlayerLineElement::render(sf::RenderWindow &target, float partialTick) {
    int alpha = 255 * (life/MAX_LIFETIME);
    sf::Color color(255,255,255, alpha);
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(startX, startY), color),
        sf::Vertex(sf::Vector2f(endX, endY), color),
    };
    target.draw(line, 2, sf::Lines);
}

PlayerLineElement::~PlayerLineElement() {
    if(rigidbody && rigidbody->GetWorld()) {
        rigidbody->GetWorld()->DestroyBody(rigidbody);
    }
}
