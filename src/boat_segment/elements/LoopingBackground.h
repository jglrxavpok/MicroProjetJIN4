//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <SFML/Graphics.hpp>
#include "SceneElement.h"
#include <memory>

/// Elément représentant un fond en scrolling
/// Utile deux sprites s'enchainant pour faire l'illusion d'un fond infini
class LoopingBackground: public SceneElement {
private:
    std::shared_ptr<sf::Texture> texture; // permet de garder la texture chargée tant qu'au moins cet élément l'utilise
    sf::Sprite spriteLeft;
    sf::Sprite spriteRight;

    void updateScale(sf::RenderTarget& target, sf::Sprite& sprite);

public:
    explicit LoopingBackground(std::shared_ptr<sf::Texture> texture);
    void update(float elapsedTime) override;

    void render(sf::RenderTarget &target, float partialTick) override;
};
