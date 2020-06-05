//
// Created by jglrxavpok on 02/06/2020.
//

#include "math_help.h"
#include "RNG.h"

bool intersectionTest(float startX, float startY, float endX, float endY, float otherStartX, float otherStartY, float otherEndX, float otherEndY, sf::Vector2f& intersectionOut) {
    float determinant = (startX-endX) * (otherStartY - otherEndY) - (startY - endY) * (otherStartX - otherEndX);
    if(determinant < 0.001f && determinant > -0.001f) {
        return false;
    }
    float tDeterminant = (startX - otherStartX) * (otherStartY - otherEndY) - (startY - otherStartY) * (otherStartX - otherEndX);
    float uDeterminant = - ((startX - endX) * (startY - otherStartY) - (startY - endY) * (startX - otherStartX));
    float divisor = (startX-endX) * (otherStartY-otherEndY) - (startY-endY) * (otherStartX-otherEndX);
    float t = tDeterminant / divisor;
    float u = uDeterminant / divisor;
    if(t > 0.0f && t < 1.0f && u > 0.0f && u < 1.0f) {
        intersectionOut.x = t*endX + (1.0f-t)*startX;
        intersectionOut.y = t*endY + (1.0f-t)*startY;
        return true;
    }
    return false;
}

static RNG rng{};
static std::vector<sf::Color> randomColors;

void fillRandomColors() {
    randomColors.push_back(sf::Color::Red);
    randomColors.push_back(sf::Color::Yellow);
    randomColors.push_back(sf::Color::Green);
    randomColors.push_back(sf::Color::White);
    randomColors.push_back(sf::Color::Cyan);
    randomColors.push_back(sf::Color::Magenta);
    //randomColors.push_back(sf::Color::Black);
    //randomColors.push_back(sf::Color::Blue);
}

sf::Color randomColor() {
    if(randomColors.empty()) {
        fillRandomColors();
    }
    return randomColors[rng.rand() * randomColors.size()];
}
