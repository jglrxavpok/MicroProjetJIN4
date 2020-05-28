//
// Created by jglrxavpok on 28/05/2020.
//

#include "BoatElement.h"
#include <math.h>

BoatElement::BoatElement(): SceneElement() {
    pointyPart.setFillColor(sf::Color::Green);
    squarePart.setFillColor(sf::Color::Green);

    pointyPart.setOrigin(pixelWidth/2.0f, pixelWidth/2.0f);
    squarePart.setOrigin(pixelLength/2.0f, pixelWidth/2.0f);
}

void BoatElement::update(float elapsedTime) {
    static float time = 0.0f;
    time += elapsedTime;
    // TODO
    position.x = 200.0f;
    position.y = sin(time) * 200.0f + 400.0f;
}

void BoatElement::render(sf::RenderWindow &target, float partialTick) {
    // TODO: more complex transform. Will need Box2D
    squarePart.setPosition(position.x, position.y);

    pointyPart.setRotation(90.0f);
    pointyPart.setPosition(position.x+pixelLength/2.0f+pixelWidth/4.0f, position.y);

    target.draw(squarePart);
    target.draw(pointyPart);
}
