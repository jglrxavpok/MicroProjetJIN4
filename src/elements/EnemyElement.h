//
// Created by jglrxavpok on 30/05/2020.
//

#pragma once
#include <memory>
#include "SceneElement.h"

class EnemyElement: public SceneElement {
private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

public:
    explicit EnemyElement(std::shared_ptr<sf::Texture> texture);

    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;
};
