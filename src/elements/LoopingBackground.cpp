//
// Created by jglrxavpok on 28/05/2020.
//
#include <memory>
#include <iostream>
#include <cmath>
#include "LoopingBackground.h"

LoopingBackground::LoopingBackground(std::shared_ptr<sf::Texture> texture): texture(texture) {
    if(texture) {
        spriteLeft.setTexture(*texture);
        spriteRight.setTexture(*texture);
    }
}

void LoopingBackground::update(float elapsedTime) {

}

/// Met à jour la taille du sprite pour correspondre à la taille de la fenêtre
void updateScale(sf::RenderWindow& target, sf::Sprite& sprite) {
    float scaleX = (float)target.getSize().x / sprite.getTexture()->getSize().x;
    float scaleY = (float)target.getSize().y / sprite.getTexture()->getSize().y;
    sprite.setScale(scaleX, scaleY);
}

void LoopingBackground::render(sf::RenderWindow &target, float partialTick) {
    updateScale(target, spriteLeft);
    updateScale(target, spriteRight);


    unsigned int screenWidth = target.getSize().x;
    position.x = fmodf(position.x, screenWidth);

    spriteLeft.setPosition(-position.x, 0);
    spriteRight.setPosition((screenWidth-position.x), 0);

    target.draw(spriteLeft);
    target.draw(spriteRight);
}
