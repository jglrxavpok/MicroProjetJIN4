//
// Created by jglrxavpok on 02/06/2020.
//

#pragma once
#include <SFML/Graphics.hpp>

bool intersectionTest(float startX, float startY, float endX, float endY, float otherStartX, float otherStartY, float otherEndX, float otherEndY, sf::Vector2f& intersectionOut);