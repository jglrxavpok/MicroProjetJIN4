//
// Created by jglrxavpok on 01/06/2020.
//

#pragma once
#include <SFML/Graphics.hpp>

class MusicLinePart {
private:
public:
    float startX;
    float startY;
    float endX;
    float endY;

    explicit MusicLinePart(float startX, float startY, float endX, float endY);

    bool intersects(MusicLinePart& other, sf::Vector2f& intersectionOut);
};
