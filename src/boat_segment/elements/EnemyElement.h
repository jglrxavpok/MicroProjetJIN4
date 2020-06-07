//
// Created by jglrxavpok on 30/05/2020.
//

#pragma once
#include <memory>
#include "SceneElement.h"
#include "Scene.h"

class EnemyElement: public SceneElement {
private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    Scene* scene;
    sf::RenderWindow& renderTarget;

    b2Body* rigidbody;

public:
    explicit EnemyElement(sf::RenderWindow& renderTarget, std::shared_ptr<sf::Texture> texture);

    void onAddition(Scene &scene) override;

    void update(float elapsedTime) override;

    void render(sf::RenderTarget &target, float partialTick) override;

    void beginContact(SceneElement *other) override;

    ~EnemyElement() override;
};
