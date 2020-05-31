//
// Created by jglrxavpok on 28/05/2020.
//

#include <elements/LoopingBackground.h>
#include "Scene.h"

Scene::Scene(unique_ptr<SceneElement> backgroundElement): background(move(backgroundElement)) {

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
