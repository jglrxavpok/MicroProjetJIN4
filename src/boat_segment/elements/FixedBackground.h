//
// Created by jglrxavpok on 5/06/2020.
//
#pragma once
#include <SFML/Graphics.hpp>
#include "SceneElement.h"
#include <memory>

/// Elément représentant un fond fixe
/// Utile deux sprites s'enchainant pour faire l'illusion d'un fond infini
class FixedBackground: public SceneElement {
private:
    std::shared_ptr<sf::Texture> texture; // permet de garder la texture chargée tant qu'au moins cet élément l'utilise
    sf::Sprite sprite;

    void updateScale(sf::RenderWindow& target, sf::Sprite& sprite);

public:
    explicit FixedBackground(std::shared_ptr<sf::Texture> texture);
    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;
};
