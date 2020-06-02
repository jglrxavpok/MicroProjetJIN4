//
// Created by jglrxavpok on 30/05/2020.
//

#include "EnemyElement.h"
#include "BoatElement.h"

EnemyElement::EnemyElement(sf::RenderWindow& renderTarget, std::shared_ptr<sf::Texture> texture): renderTarget(renderTarget), texture(texture) {
    if(texture) sprite.setTexture(*texture);
}

void EnemyElement::update(float elapsedTime) {
    auto cameraCoords = renderTarget.mapCoordsToPixel(position, scene->getRenderView());
    if(cameraCoords.x < -200.0f) {
        // on retire les ennemis qui sont sortis de l'écran
        scheduleForRemoval();
    }
}

void EnemyElement::render(sf::RenderWindow &target, float partialTick) {
    sprite.setPosition(position.x-sprite.getTexture()->getSize().x/2.0f, position.y-sprite.getTexture()->getSize().y/2.0f);
    target.draw(sprite);
}

void EnemyElement::onAddition(Scene &scene) {
    this->scene = &scene;

    b2BodyDef bodyDef;
    bodyDef.userData = this;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.x = position.x;
    bodyDef.position.y = position.y;

    rigidbody = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(10.0f, 10.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.shape = &shape;
    //fixtureDef.isSensor = true;
    rigidbody->CreateFixture(&fixtureDef);
}

void EnemyElement::beginContact(SceneElement *other) {
    if(shouldBeRemoved())
        return;
    if(auto* player = dynamic_cast<BoatElement*>(other)) {
        player->damage(1.0f);
        this->scheduleForRemoval();
    }
}

EnemyElement::~EnemyElement() {
    if(rigidbody && rigidbody->GetWorld()) {
        rigidbody->GetWorld()->DestroyBody(rigidbody);
    }
}
