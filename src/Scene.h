//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "elements/SceneElement.h"
#include <box2d/box2d.h>

using namespace std;

/// Renseigne les éléments à mettre à jour et à afficher
class Scene {
private:
    // valeurs recommandées dans la documentation de Box2D
    static constexpr int positionIterations = 3;
    static constexpr int velocityIterations = 8;

    unique_ptr<SceneElement> background = nullptr;
    vector<unique_ptr<SceneElement>> elements;
    b2World physics{ b2Vec2_zero /* aucune gravité pour nous, on a une vue du dessus */ };

public:
    explicit Scene(unique_ptr<SceneElement> background);

    void addElement(unique_ptr<SceneElement>&& element);
    void renderAll(sf::RenderWindow& target, float partialTick);
    void updateAll(float elapsedTime);

    b2World& getPhysicsWorld();
};
