//
// Created by jglrxavpok on 28/05/2020.
//

#include "SceneElement.h"

sf::Vector2f& SceneElement::getPosition() {
    return position;
}

void SceneElement::scheduleForRemoval() {
    remove = true;
}

bool SceneElement::shouldBeRemoved() {
    return remove;
}