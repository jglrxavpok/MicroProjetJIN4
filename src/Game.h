//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <SFML/Graphics.hpp>

/// Appelé toutes les TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
void update();

/// Appelé autant que possible pour afficher le jeu
/// @param renderTarget fenêtre sur laquelle on dessine
/// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
/// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
void render(sf::RenderWindow& renderTarget, float partialTick);