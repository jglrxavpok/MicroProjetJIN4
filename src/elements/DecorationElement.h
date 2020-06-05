//
// Created by jglrxavpok on 05/06/2020.
//

#pragma once
#include "SceneElement.h"
#include <memory>

/// Elément dont le seul but est d'afficher une texture
class DecorationElement: public SceneElement {
private:
    sf::Sprite sprite;
    std::shared_ptr<sf::Texture> texture;

public:
    explicit DecorationElement(std::shared_ptr<sf::Texture> texture, float x, float y);

    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;
};
