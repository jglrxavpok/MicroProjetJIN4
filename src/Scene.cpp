//
// Created by jglrxavpok on 28/05/2020.
//

#include <elements/LoopingBackground.h>
#include <elements/BoatElement.h>
#include <functional>
#include "Scene.h"

Scene::Scene(unique_ptr<SceneElement> backgroundElement): background(move(backgroundElement)) {
    getPhysicsWorld().SetContactListener(this);
}

void Scene::renderAll(sf::RenderWindow &target, float partialTick) {
    auto& defaultView = target.getDefaultView();
    // le fond n'est pas affecté par la vue
    if(background) {
        background->render(target, partialTick);
    }
    target.setView(renderView);

    for(auto& element : elements) {
        element->render(target, partialTick);
    }

    target.setView(defaultView);
}

void Scene::updateAll(float elapsedTime) {
    if(background) {
        background->update(elapsedTime);
    }

    physics.Step(elapsedTime, velocityIterations, positionIterations);

    for(auto& element : elements) {
        element->update(elapsedTime);
    }
    elements.erase(remove_if(elements.begin(), elements.end(), [](const auto& elem) { return elem->shouldBeRemoved(); }), elements.end());
}

void Scene::addElement(unique_ptr<SceneElement>&& element) {
    // on enregistre le dernier joueur ajouté à la scène pour pouvoir l'utiliser dans getPlayer()
    // cf. lastPlayerAdded
    if(auto* player = dynamic_cast<BoatElement*>(element.get())) {
        lastPlayerAdded = player;
    }

    element->onAddition(*this);
    elements.push_back(move(element));
}

b2World& Scene::getPhysicsWorld() {
    return physics;
}

sf::View& Scene::getRenderView() {
    return renderView;
}

unique_ptr<SceneElement>& Scene::getBackground() {
    return background;
}

vector<unique_ptr<SceneElement>>& Scene::getElements() {
    return elements;
}

BoatElement& Scene::getPlayer() {
    if(lastPlayerAdded) {
        return *lastPlayerAdded;
    }
    throw std::logic_error("Called getPlayer while none has been added");
}

template<typename Callback>
void handleContact(b2Contact* contact, Callback callback) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();
    if(bodyA && bodyB) {
        void* dataA = bodyA->GetUserData();
        void* dataB = bodyB->GetUserData();
        if(dataA && dataB) {
            auto* elemA = (SceneElement*) dataA;
            auto* elemB = (SceneElement*) dataB;
            callback(elemA, elemB);
            callback(elemB, elemA);
        }
    }
}

Scene::~Scene() {
    propagateContacts = false;
}

void Scene::BeginContact(b2Contact *contact) {
    if(propagateContacts) {
        handleContact(contact, [](SceneElement* a, SceneElement* b) { a->beginContact(b); b->beginContact(a); });
    }
}

void Scene::EndContact(b2Contact *contact) {
    if(propagateContacts) {
        handleContact(contact, [](SceneElement* a, SceneElement* b) { a->endContact(b); b->endContact(a); });
    }
}

void Scene::disableContactPropagationForTesting() {
    propagateContacts = false;
}
