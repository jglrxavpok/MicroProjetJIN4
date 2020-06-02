//
// Created by jglrxavpok on 30/05/2020.
//

#include "EnemyElement.h"

EnemyElement::EnemyElement(std::shared_ptr<sf::Texture> texture): texture(texture) {
    if(texture) sprite.setTexture(*texture);
}

void EnemyElement::update(float elapsedTime) {
    // TODO
}

void EnemyElement::render(sf::RenderWindow &target, float partialTick) {
    sprite.setPosition(position.x-sprite.getTexture()->getSize().x/2.0f, position.y-sprite.getTexture()->getSize().y/2.0f);
    target.draw(sprite);
}
