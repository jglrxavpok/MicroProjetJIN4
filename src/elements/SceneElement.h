//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <SFML/Graphics.hpp>

class Scene;

class SceneElement {
protected:
    sf::Vector2f position;

public:
    explicit SceneElement() = default;

    virtual void onAddition(Scene& scene) {};
    virtual void update(float elapsedTime) = 0;
    virtual void render(sf::RenderWindow& target, float partialTick) = 0;
    virtual ~SceneElement() = default;
};
