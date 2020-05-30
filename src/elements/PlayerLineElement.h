//
// Created by jglrxavpok on 29/05/2020.
//

#pragma once

#include "SceneElement.h"
#include <box2d/box2d.h>
#include <memory>
#include "MusicLine.h"

class PlayerLineElement: public SceneElement {
private:
    float length;
    float startX;
    float startY;
    float endX;
    float endY;
    b2Body* rigidbody;

    float life;
    /// ligne à laquelle cette élément appartient.
    /// le shared_ptr permet de détruire la MusicLine une fois que tous ses enfants sont détruits
    std::shared_ptr<MusicLine> parent;

public:
    constexpr static float MAX_LIFETIME = 5.0f; // 5s
    explicit PlayerLineElement(std::shared_ptr<MusicLine> parent, float startX, float startY, float endX, float endY);

    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;

    void onAddition(Scene &scene) override;

    ~PlayerLineElement();
};
