//
// Created by jglrxavpok on 29/05/2020.
//

#pragma once

#include "SceneElement.h"
#include <box2d/box2d.h>
#include <memory>
#include "boat_segment/MusicLine.h"

class MusicLineElement: public SceneElement {
private:
    float length;
    float startX;
    float startY;
    float endX;
    float endY;
    b2Body* rigidbody;

    float life;
    /// ligne à laquelle cette élément appartient.
    /// le shared_ptr permet de détruire la MusicLine une fois que tous ses enfants sont détruits
    std::shared_ptr<MusicLine> parent;

    /// morceau de la ligne correspondant à cet élément. Permet à la MusicLine parente de savoir si cet élément a expiré
    std::shared_ptr<MusicLinePart> part;
    std::shared_ptr<sf::Texture> spritesheet;
    vector<sf::Sprite> sprites;
    sf::Color noteColor{0,0,0,255};

    constexpr static float SPRITE_WIDTH = 64.0f;

public:
    constexpr static float MAX_LIFETIME = 5.0f; // 5s
    explicit MusicLineElement(std::shared_ptr<MusicLine> parent, std::shared_ptr<MusicLinePart> part, float startX, float startY, float endX, float endY, std::shared_ptr<sf::Texture> = nullptr);

    void update(float elapsedTime) override;

    void render(sf::RenderWindow &target, float partialTick) override;

    void onAddition(Scene &scene) override;

    ~MusicLineElement() override;

    void beginContact(SceneElement *other) override;

    /// Appelé lorsqu'une ligne se brise sur un ennemi
    void breakPart();
};
