//
// Created by jglrxavpok on 05/06/2020.
//
#include <memory>
#include "FixedBackground.h"

FixedBackground::FixedBackground(std::shared_ptr<sf::Texture> texture): texture(texture) {
    if(texture) {
        sprite.setTexture(*texture);
    }
}

void FixedBackground::update(float elapsedTime) {

}

/// Met à jour la taille du sprite pour correspondre à la taille de la fenêtre
void FixedBackground::updateScale(sf::RenderWindow& target, sf::Sprite& sprite) {
    float scaleX = (float)target.getSize().x / sprite.getTexture()->getSize().x;
    float scaleY = (float)target.getSize().y / sprite.getTexture()->getSize().y;
    sprite.setScale(scaleX, scaleY);
}

void FixedBackground::render(sf::RenderWindow &target, float partialTick) {
    updateScale(target, sprite);
    sprite.setPosition(0, 0);
    target.draw(sprite);
}
