//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <SFML/Graphics.hpp>

class Scene;

class SceneElement {
private:
    bool remove = false;

protected:
    sf::Vector2f position;

public:
    explicit SceneElement() = default;

    virtual void onAddition(Scene& scene) {};
    virtual void update(float elapsedTime) = 0;
    virtual void render(sf::RenderTarget &target, float partialTick) = 0;
    virtual ~SceneElement() = default;

    void scheduleForRemoval();
    bool shouldBeRemoved();

    sf::Vector2f& getPosition();

    virtual void beginContact(SceneElement* other);
    virtual void endContact(SceneElement* other);
};
