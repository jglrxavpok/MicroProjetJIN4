//
// Created by jglrxavpok on 30/05/2020.
//

#include "EnemyElement.h"

EnemyElement::EnemyElement(sf::RenderWindow& renderTarget, std::shared_ptr<sf::Texture> texture): renderTarget(renderTarget), texture(texture) {
    if(texture) sprite.setTexture(*texture);
}

void EnemyElement::update(float elapsedTime) {
    auto cameraCoords = renderTarget.mapCoordsToPixel(position, scene->getRenderView());
    if(cameraCoords.x < -200.0f) {
        // on retire les ennemis qui sont sortis de l'écran
        scheduleForRemoval();
    }
}

void EnemyElement::render(sf::RenderWindow &target, float partialTick) {
    sprite.setPosition(position.x-sprite.getTexture()->getSize().x/2.0f, position.y-sprite.getTexture()->getSize().y/2.0f);
    target.draw(sprite);
}

void EnemyElement::onAddition(Scene &scene) {
    this->scene = &scene;
}
