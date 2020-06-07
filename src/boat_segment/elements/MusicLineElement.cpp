//
// Created by jglrxavpok on 29/05/2020.
//

#include "MusicLineElement.h"

#include <utility>
#include "Scene.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "math_help.h"


MusicLineElement::MusicLineElement(std::shared_ptr<MusicLine> parent, std::shared_ptr<MusicLinePart> part, float startX, float startY, float endX, float endY, std::shared_ptr<sf::Texture> spritesheet):
    parent(std::move(parent)), part(std::move(part)), startX(startX), startY(startY), endX(endX), endY(endY), spritesheet(spritesheet) {
    float dx = endX-startX;
    float dy = endY-startY;
    length = sqrt(dx*dx+dy*dy);

    life = MAX_LIFETIME;

    noteColor = randomColor();

    if(spritesheet) {
        int width = spritesheet->getSize().x;
        int count = width/SPRITE_WIDTH;
        for (int i = 0; i < count; ++i) {
            sf::Sprite sprite{};
            sf::IntRect rectangle(sf::Vector2i(i*SPRITE_WIDTH, 0), sf::Vector2i(SPRITE_WIDTH, SPRITE_WIDTH));
            sprite.setTextureRect(rectangle);
            sprite.setTexture(*spritesheet);
            sprite.setColor(noteColor);
            sprites.push_back(sprite);
        }
    }
}

void MusicLineElement::onAddition(Scene &scene) {
    b2BodyDef bodyDef;
    bodyDef.userData = this;
    bodyDef.type = b2_kinematicBody;
    bodyDef.angle = atan2((endY-startY), endX-startX);

    bodyDef.position.Set((startX+endX)/2.0f, (startY+endY)/2.0f);
    rigidbody = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape line;
    line.SetAsBox(length/2.0f, 10.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &line;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    rigidbody->CreateFixture(&fixtureDef);
}

void MusicLineElement::update(float elapsedTime) {
    life -= elapsedTime;
    if(life <= 0) {
        scheduleForRemoval();
        return;
    }

    if(part->isBroken()) {
        breakPart();
    }
}

void MusicLineElement::render(sf::RenderTarget &target, float partialTick) {
    int alpha = 255 * (life/MAX_LIFETIME);

    if(spritesheet) {
        float animationSpeed = 0.1f;
        int animationIndex = (int) (life / animationSpeed) % sprites.size();
        sf::Sprite& sprite = sprites[animationIndex];

        sf::Color col = sprite.getColor();
        col.a = alpha;
        sprite.setColor(col);
        sprite.setPosition((startX+endX)/2.0f - SPRITE_WIDTH/2.0f, (startY+endY)/2.0f - SPRITE_WIDTH/2.0f);
        target.draw(sprite);
    }
}

void MusicLineElement::beginContact(SceneElement *other) {
    if(auto* enemy = dynamic_cast<EnemyElement*>(other)) {
        parent->breakLine();
    }
}

MusicLineElement::~MusicLineElement() {
    if(rigidbody && rigidbody->GetWorld()) {
        rigidbody->GetWorld()->DestroyBody(rigidbody);
    }
}

void MusicLineElement::breakPart() {
    // TODO: feedback visuel: utiliser scene pour positioner des éléments "particule" si on a le temps?
    scheduleForRemoval();
}
