//
// Created by jglrxavpok on 28/05/2020.
//

#include <elements/LoopingBackground.h>
#include "Scene.h"

Scene::Scene(unique_ptr<SceneElement> backgroundElement): background(move(backgroundElement)) {

}

void Scene::renderAll(sf::RenderWindow &target, float partialTick) {
    if(background) {
        background->render(target, partialTick);
    }

    for(auto& element : elements) {
        element->render(target, partialTick);
    }
}

void Scene::updateAll(float elapsedTime) {
    if(background) {
        background->update(elapsedTime);
    }

    for(auto& element : elements) {
        element->update(elapsedTime);
    }
}

void Scene::addElement(unique_ptr<SceneElement>&& element) {
    elements.push_back(move(element));
}
