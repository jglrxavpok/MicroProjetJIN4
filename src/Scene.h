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
class Scene: public enable_shared_from_this<Scene> {
private:
    // valeurs recommandées dans la documentation de Box2D
    static constexpr int positionIterations = 3;
    static constexpr int velocityIterations = 8;

    unique_ptr<SceneElement> background = nullptr;

    // attention l'ordre est important, les destructeurs détruisent dans l'ordre inverse de déclaration, et des éléments
    // peuvent vouloir libérer un b2Body (ou autre) dans leur destructeur. 'physics' doit donc être valide à cet instant
    b2World physics{ b2Vec2_zero /*b2Vec2(100.0f, 0.0f) */ };
    vector<unique_ptr<SceneElement>> elements;
    sf::View renderView{sf::FloatRect(0.0f, 0.0f, 1600.0f, 900.0f)};

public:
    explicit Scene(unique_ptr<SceneElement> background);

    void addElement(unique_ptr<SceneElement>&& element);
    void renderAll(sf::RenderWindow& target, float partialTick);
    void updateAll(float elapsedTime);

    b2World& getPhysicsWorld();
    sf::View& getRenderView();
    unique_ptr<SceneElement>& getBackground();
    vector<unique_ptr<SceneElement>>& getElements();
};
