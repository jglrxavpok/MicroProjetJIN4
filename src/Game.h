//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace Game {
    /// 60Hz
    constexpr float TARGET_UPDATE_PERIOD = 1.0/60.0;

    /// Appelé au démarrage du jeu
    void init();

    /// Appelé toutes les TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
    void update();

    /// Appelé autant que possible pour afficher le jeu
    /// @param renderTarget fenêtre sur laquelle on dessine
    /// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
    /// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
    void render(sf::RenderWindow& renderTarget, float partialTick);

    std::shared_ptr<sf::Texture> loadTexture(std::string path);
}
