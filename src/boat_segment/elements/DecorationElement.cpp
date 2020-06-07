//
// Created by jglrxavpok on 05/06/2020.
//

#include "DecorationElement.h"

void DecorationElement::update(float elapsedTime) {}

DecorationElement::DecorationElement(std::shared_ptr<sf::Texture> texture, float x, float y): DecorationElement(texture, x, y, texture->getSize().x, texture->getSize().y) {}

DecorationElement::DecorationElement(std::shared_ptr<sf::Texture> texture, float x, float y, float w, float h): texture(texture) {
    sprite.setTexture(*texture);
    sprite.setPosition(x, y);
    sprite.setScale(w/texture->getSize().x, h/texture->getSize().y);
}

void DecorationElement::render(sf::RenderTarget &target, float partialTick) {
    target.draw(sprite);
}
