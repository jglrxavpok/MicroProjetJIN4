//
// Created by jglrxavpok on 05/06/2020.
//

#include "FinishLineElement.h"
#include "BoatElement.h"
#include "boat_segment/BoatSegment.h"
#include <iostream>

FinishLineElement::FinishLineElement(const tmx::Object &source): sourceObject(source) {

}

void FinishLineElement::onAddition(Scene& scene) {
    b2BodyDef def;
    def.type = b2_staticBody;
    def.userData = this;
    rigidbody = scene.getPhysicsWorld().CreateBody(&def);

    b2Fixture* fixture = BoatSegment::createFixture(rigidbody, sourceObject);
    fixture->SetSensor(true);
}

void FinishLineElement::update(float elapsedTime) {}

void FinishLineElement::render(sf::RenderWindow &target, float partialTick) {
    // le rendu est géré avec l'image layer dans la map faite avec Tiled
}

void FinishLineElement::beginContact(SceneElement *other) {
    if(auto* player = dynamic_cast<BoatElement*>(other)) {
        // TODO: next gameplay segment
        cout << "next gameplay!!" << endl;
    }
}

FinishLineElement::~FinishLineElement() {
    if(rigidbody && rigidbody->GetWorld()) {
        rigidbody->GetWorld()->DestroyBody(rigidbody);
    }
}
