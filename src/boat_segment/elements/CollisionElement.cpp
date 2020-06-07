//
// Created by jglrxavpok on 06/06/2020.
//

#include "CollisionElement.h"
#include "Scene.h"
#include "boat_segment/BoatSegment.h"

CollisionElement::CollisionElement(const tmx::Object &source): sourceObject(source) {

}

void CollisionElement::onAddition(Scene& scene) {
    b2BodyDef def;
    def.type = b2_staticBody;
    def.userData = this;
    rigidBody = scene.getPhysicsWorld().CreateBody(&def);

    BoatSegment::createFixture(rigidBody, sourceObject);
}

void CollisionElement::update(float elapsedTime) {

}

void CollisionElement::render(sf::RenderTarget &target, float partialTick) {

}

CollisionElement::~CollisionElement() {
    if(rigidBody && rigidBody->GetWorld()) {
        rigidBody->GetWorld()->DestroyBody(rigidBody);
    }
}

