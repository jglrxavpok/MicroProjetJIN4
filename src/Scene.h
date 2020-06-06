//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "boat_segment/elements/SceneElement.h"
#include <box2d/box2d.h>

using namespace std;

class BoatElement;

/// Renseigne les éléments à mettre à jour et à afficher
class Scene: public enable_shared_from_this<Scene>, b2ContactListener {
private:
    // valeurs recommandées dans la documentation de Box2D
    static constexpr int positionIterations = 3;
    static constexpr int velocityIterations = 8;

    /// La scène doit-elle propager les informations de contact aux éléments? Désactivé lors du destructeur pour éviter
    /// des accès mémoires incorrects
    bool propagateContacts = true;

    /// Dernier joueur ajouté à la scène, utilisé par getPlayer. On se permet un pointeur nu car seul Scene y aura accès
    /// et que le BoatElement appartient à 'elements'. Cf. getPlayer()
    BoatElement* lastPlayerAdded = nullptr;

    unique_ptr<SceneElement> background = nullptr;

    // attention l'ordre est important, les destructeurs détruisent dans l'ordre inverse de déclaration, et des éléments
    // peuvent vouloir libérer un b2Body (ou autre) dans leur destructeur. 'physics' doit donc être valide à cet instant
    b2World physics{ b2Vec2_zero /*b2Vec2(100.0f, 0.0f) */ };
    vector<unique_ptr<SceneElement>> elements;

    sf::View renderView{sf::FloatRect(0.0f, 0.0f, 1600.0f, 900.0f)};

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* pair) override;

public:
    explicit Scene(unique_ptr<SceneElement> background);

    void addElement(unique_ptr<SceneElement>&& element);
    void renderAll(sf::RenderWindow& target, float partialTick);
    void updateAll(float elapsedTime);

    b2World& getPhysicsWorld();
    sf::View& getRenderView();
    unique_ptr<SceneElement>& getBackground();
    vector<unique_ptr<SceneElement>>& getElements();

    /// Récupère le premier joueur (BoatElement) présent dans getElements()
    /// Throw si aucun joueur n'est présent
    BoatElement& getPlayer();

    /// Ne doit être appelé que par les tests unitaires pour s'assurer que les contacts ne sont pas pris en compte
    void disableContactPropagationForTesting();

    ~Scene() override;
};
