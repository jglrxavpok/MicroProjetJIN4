//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once

#include "SceneElement.h"
#include <SFML/Graphics.hpp>

class BoatElement: public SceneElement {
private:
    constexpr static float pixelWidth = 80.0f;
    constexpr static float pixelLength = 200.0f;
    sf::RectangleShape squarePart{sf::Vector2f{pixelLength, pixelWidth}};
    sf::CircleShape pointyPart{pixelWidth/2.0f, 3};

public:
    explicit BoatElement();

    void update(float elapsedTime) override;
    void render(sf::RenderWindow &target, float partialTick) override;
};
