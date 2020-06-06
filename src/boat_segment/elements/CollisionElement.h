//
// Created by jglrxavpok on 06/06/2020.
//

#pragma once
#include "SceneElement.h"
#include <tmxlite/Object.hpp>
#include <box2d/box2d.h>

class CollisionElement: public SceneElement {
private:
    const tmx::Object& sourceObject;
    b2Body* rigidBody = nullptr;

public:
    explicit CollisionElement(const tmx::Object& sourceObject);

    void onAddition(Scene &scene) override;

    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;

    ~CollisionElement() override;
};
