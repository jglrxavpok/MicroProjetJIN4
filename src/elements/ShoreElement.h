//
// Created by jglrxavpok on 29/05/2020.
//
#pragma once

#include <memory>
#include "SceneElement.h"

class ShoreElement: public SceneElement {
private:
    float topY;

public:
    constexpr static float HEIGHT = 83.0f;

    explicit ShoreElement(float topY);
    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;

    void onAddition(Scene &scene) override;
};
