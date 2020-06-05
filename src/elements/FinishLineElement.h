//
// Created by jglrxavpok on 05/06/2020.
//

#pragma once
#include "SceneElement.h"
#include <tmxlite/Object.hpp>
#include <box2d/box2d.h>

class FinishLineElement: public SceneElement {
private:
    const tmx::Object& sourceObject;
    b2Body* rigidbody;

public:
    explicit FinishLineElement(const tmx::Object& source);

    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;

    void beginContact(SceneElement *other) override;

    ~FinishLineElement() override;

    void onAddition(Scene &scene) override;
};

