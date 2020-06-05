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

    if(rigidbody) {
        auto transform = rigidbody->GetTransform();
        position.x = transform.p.x;
        position.y = transform.p.y;

        // on ne bouge que si le joueur peut voir l'ennemi, c'est pas juste sinon
        if(cameraCoords.x > 0 && cameraCoords.x < scene->getRenderView().getSize().x
        && cameraCoords.y > 0 && cameraCoords.y < scene->getRenderView().getSize().y) {
            // approche l'ennemi du joueur
            BoatElement& player = scene->getPlayer();
            b2Vec2 force;
            const float intensity = 100.0f * rigidbody->GetMass();
            float dx = getPosition().x - player.getPosition().x;
            float dy = getPosition().y - player.getPosition().y;
            force.x = -dx;
            force.y = -dy;
            force.Normalize();
            force *= intensity;
            rigidbody->ApplyForceToCenter(force, true);
        }
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
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.x = position.x;
    bodyDef.position.y = position.y;

    rigidbody = scene.getPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(10.0f, 10.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.shape = &shape;
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
