//
// Created by jglrxavpok on 29/05/2020.
//

#pragma once

#include "SceneElement.h"
#include <box2d/box2d.h>

class PlayerLineElement: public SceneElement {
private:
    constexpr static float MAX_LIFETIME = 5.0f; // 5s
    float length;
    float startX;
    float startY;
    float endX;
    float endY;
    b2Body* rigidbody;

    float life;

public:
    explicit PlayerLineElement(float startX, float startY, float endX, float endY);

    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;

    void onAddition(Scene &scene) override;

    ~PlayerLineElement();
};
