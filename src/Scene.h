//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "elements/SceneElement.h"

using namespace std;

/// Renseigne les éléments à mettre à jour et à afficher
class Scene {
private:
    unique_ptr<SceneElement> background = nullptr;
    vector<unique_ptr<SceneElement>> elements;

public:
    explicit Scene(unique_ptr<SceneElement> background);

    void addElement(unique_ptr<SceneElement>&& element);
    void renderAll(sf::RenderWindow& target, float partialTick);
    void updateAll(float elapsedTime);
};
