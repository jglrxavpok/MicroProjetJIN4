//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once

#include "SceneElement.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/box2d.h>
#include <memory>
#include "boat_segment/GameState.h"

class BoatElement: public SceneElement {
private:
    /// en radians
    float angle = 0.0f;
    constexpr static float pixelWidth = 80.0f;
    constexpr static float pixelLength = 200.0f;
    sf::RectangleShape squarePart{sf::Vector2f{pixelLength, pixelWidth}};
    sf::CircleShape pointyPart{pixelWidth/2.0f, 3};

    /// pointeur nu car Box2D gère toute la mémoire par lui-même et que le destructeur de b2Body est privé
    b2Body* rigidbody = nullptr; // TODO: trouver comment nettoyer quand on en a plus besoin

    /// on peut se permettre un pointeur nu car l'élément a une durée de vie plus courte que la Scene.
    /// De plus, utiliser un weak_ptr impose que Scene soit possédé dans un shared_ptr, ce qui n'est pas le cas,
    /// une Scene étant supposée possédée uniquement dans la namespace Game
    Scene* scene = nullptr;

    /// contrôle l'intensité appliquée à l'impulsion pour redresser le bateau
    float angularImpulseFactor = 0.05f;

    /// contrôle l'intensité appliquée à l'impulsion pour faire avancer le bateau
    float linearImpulseFactor = 10.0f;

    /// contrôle la réduction de vitesse de rotation
    float angularDamping = 0.75f;

    /// son à jouer quand le joueur prend des dégâts
    std::shared_ptr<sf::Sound> hurtSound;

    GameState& gameState;

public:
    explicit BoatElement(GameState& gameState, std::shared_ptr<sf::Sound> hurtSound);

    void update(float elapsedTime) override;
    void render(sf::RenderTarget &target, float partialTick) override;

    void onAddition(Scene &scene) override;

    void damage(float amount);
};
