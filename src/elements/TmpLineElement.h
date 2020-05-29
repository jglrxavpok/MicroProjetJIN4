//
// Created by jglrxavpok on 29/05/2020.
//

#pragma once

#include "SceneElement.h"
#include <box2d/box2d.h>

// FIXME: TMP
class TmpLineElement: public SceneElement {
private:
    float startX;
    float startY;
    b2Body* rigidbody;

public:
    explicit TmpLineElement(float startX, float startY);

    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;

    void onAddition(Scene &scene) override;
};
