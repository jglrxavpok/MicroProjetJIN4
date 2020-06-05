//
// Created by jglrxavpok on 05/06/2020.
//

#include "DecorationElement.h"

void DecorationElement::update(float elapsedTime) {}

void DecorationElement::render(sf::RenderWindow &target, float partialTick) {
    target.draw(sprite);
}

DecorationElement::DecorationElement(std::shared_ptr<sf::Texture> texture, float x, float y): texture(texture) {
    sprite.setTexture(*texture);
    sprite.setPosition(x, y);
}
